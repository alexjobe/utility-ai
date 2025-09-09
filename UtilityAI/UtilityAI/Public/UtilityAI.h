#pragma once
#include "Math/Curves.h"
#include "Math/Math.h"
#include <algorithm>
#include <cmath>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

namespace UtilityAI
{
	struct UTAgentContext 
	{
		std::unordered_map<std::string, float> Needs;
		std::unordered_map<std::string, float> Stats;

		float GetNeed(const std::string& Key) const
		{
			if (auto It = Needs.find(Key); It != Needs.end())
			{
				return It->second;
			}
			return 0.f;
		}

		float GetStat(const std::string& Key) const
		{
			if (auto It = Stats.find(Key); It != Stats.end())
			{
				return It->second;
			}
			return 0.f;
		}
	};

	struct UTEvaluationParams
	{
		std::string Target; // e.g. "Wealth", "Survival"
		float Magnitude = 0.0f;
		float MinRaw = 0.0f;
		float MaxRaw = 1.0f;
		float Weight = 1.0f;
	};

	struct UTConsideration 
	{
		std::string Key;
		std::function<float(const UTAgentContext&, const UTEvaluationParams&)> EvalRawScore;
		float RawScore = 0.f; // Used if EvalRawScore is null
		CurveFn ScoreCurve;
		int CurvePriority = 0;
		UTEvaluationParams Params;

		float Score(const UTAgentContext& Context) const 
		{
			float Raw = EvalRawScore ? EvalRawScore(Context, Params) : RawScore;
			Raw = std::clamp(Raw, 0.f, 1.f);
			return ScoreCurve ? ScoreCurve(Raw) : Raw;
		}
	};

	class UTEffect
	{
	public:
		std::string Name;

		std::function<float(const UTAgentContext&, const UTEvaluationParams&)> EvalRawScore;
		float RawScore = 0.f; // Used if EvalRawScore is null
		CurveFn ScoreCurve;
		int CurvePriority = 0;
		UTEvaluationParams Params;

		bool bIsConsideration = true;

		// For custom effects
		std::function<void(const UTAgentContext&, const UTEvaluationParams&)> CustomFn;

		virtual void Apply(UTAgentContext& Context) const
		{
			if (CustomFn) CustomFn(Context, Params);
		}

		virtual UTConsideration AsConsideration() const
		{
			return { "Effect_" + Name, EvalRawScore, RawScore, ScoreCurve, CurvePriority, Params };
		}
	};

	class UTEffect_Need : public UTEffect
	{
	public:
		static float ScoreNeedChange(const UTAgentContext& Context, const UTEvaluationParams& Params)
		{
			const float Before = Context.GetNeed(Params.Target);

			const float After = std::clamp(Before - Params.Magnitude, Params.MinRaw, Params.MaxRaw);
			const float PercentChange = (After - Before) / (Params.MaxRaw - Params.MinRaw);

			const float BeforeNorm = Normalize(Before, Params.MinRaw, Params.MaxRaw);
			const float RawScore = BeforeNorm * -PercentChange; // Favor smaller values

			return std::clamp(RawScore, 0.0f, 1.0f);
		}

		virtual void Apply(UTAgentContext& Context) const override
		{
			Context.Needs[Params.Target] -= Params.Magnitude;
			Context.Needs[Params.Target] = std::clamp(Context.Needs[Params.Target], Params.MinRaw, Params.MaxRaw);
		}

		virtual UTConsideration AsConsideration() const
		{
			return { "Need_" + Params.Target, UTEffect_Need::ScoreNeedChange, RawScore, ScoreCurve, CurvePriority, Params };
		}
	};

	struct UTAction 
	{
		std::string Name;
		std::vector<std::string> Tags;
		std::vector<UTEffect> Effects;
		std::unordered_map<std::string, UTConsideration> Considerations;

		void AddConsideration(const UTConsideration& NewCons)
		{
			auto It = Considerations.find(NewCons.Key);
			if (It != Considerations.end())
			{
				// Merge into existing consideration
				It->second.Params.Weight *= NewCons.Params.Weight;
				if (NewCons.ScoreCurve && NewCons.CurvePriority >= It->second.CurvePriority)
				{
					It->second.ScoreCurve = NewCons.ScoreCurve;
					It->second.CurvePriority = NewCons.CurvePriority;
				}
			}
			else
			{
				Considerations[NewCons.Key] = NewCons;
			}
		}

		// Generate considerations from effects
		void GenerateConsiderations() 
		{
			for (auto& Effect : Effects) 
			{
				if (Effect.bIsConsideration) 
				{
					AddConsideration(Effect.AsConsideration());
				}
			}
		}

		// Weighted geometric mean (log-sum)
		float Score(const UTAgentContext& Context) const 
		{
			if (Considerations.empty()) return 0.0f;

			float WeightedLogSum = 0.0f;
			float WeightSum = 0.0f;

			for (const auto& [Key, Consideration] : Considerations)
			{
				const float ConScore = std::clamp(Consideration.Score(Context), 0.001f, 1.0f); // avoid log(0)
				WeightedLogSum += Consideration.Params.Weight * std::log(ConScore);
				WeightSum += Consideration.Params.Weight;
			}

			return std::exp(WeightedLogSum / WeightSum);
		}

		void Execute(UTAgentContext& Context) 
		{
			for (auto& Effect : Effects) 
			{
				Effect.Apply(Context);
			}
		}
	};

	// Biases add their effects and considerations to actions with the required tags
	struct UTBias
	{
		std::vector<std::string> RequiredTags;
		std::vector<UTEffect> Effects;
		std::vector<UTConsideration> Considerations;

		bool AppliesTo(const UTAction& Action) const 
		{
			if (RequiredTags.empty()) return true;
			for (auto& Tag : RequiredTags) 
			{
				if (std::find(Action.Tags.begin(), Action.Tags.end(), Tag) != Action.Tags.end())
					return true;
			}
			return false;
		}
	};

	struct UTAgentTrait
	{
		std::string Name;
		std::vector<UTBias> Biases;
	};

	inline void ApplyBiases(UTAction& Action, const std::vector<UTAgentTrait>& Traits)
	{
		for (const auto& Trait : Traits)
		{
			for (const auto& Bias : Trait.Biases)
			{
				for (const auto& BiasCons : Bias.Considerations)
				{
					Action.AddConsideration(BiasCons);
				}

				for (const auto& BiasEffect : Bias.Effects)
				{
					Action.Effects.push_back(BiasEffect);
				}
			}
		}
	}
}