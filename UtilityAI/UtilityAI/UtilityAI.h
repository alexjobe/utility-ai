#pragma once
#include "Math/Curves.h"
#include "Math/Math.h"
#include <algorithm>
#include <cmath>
#include <functional>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

namespace UtilityAI
{
	struct UtCharacterContext 
	{
		std::unordered_map<std::string, float> Needs;
		std::unordered_map<std::string, float> Stats;
	};

	struct UtConsideration 
	{
		std::string Name;
		std::function<float(const UtCharacterContext&)> EvalRaw;  // Hook into game state
		CurveFn Curve;
		float Magnitude = 0.0f; // Delta for needs/status
		float MinRaw = 0.0f;
		float MaxRaw = 1.0f;
		float Weight = 1.0f;
		int CurvePriority = 0;

		float Score(const UtCharacterContext& Context) const 
		{
			float Raw = std::clamp(EvalRaw ? EvalRaw(Context) : Magnitude, MinRaw, MaxRaw);
			Raw = std::clamp(Raw, MinRaw, MaxRaw);
			float Norm = Normalize(Raw, MinRaw, MaxRaw);
			return Curve ? Curve(Norm) : Norm;
		}
	};

	struct UtEffect
	{
		enum class EEffectType { NeedChange, StatusChange, Custom };

		std::string Name;
		EEffectType Type;
		std::string Target;  // e.g. "Wealth", "Survival"

		// Optional consideration
		std::optional<UtConsideration> Consideration;

		// For custom effects
		std::function<void(UtCharacterContext&)> CustomFn;

		void Apply(UtCharacterContext& Context)
		{
			switch (Type) {
			case EEffectType::NeedChange:
				Context.Needs[Target] += Consideration ? Consideration->Magnitude : 0.0f;
				Context.Needs[Target] = std::clamp(Context.Needs[Target], 0.0f, 1.0f);
				break;

			case EEffectType::StatusChange:
				Context.Stats[Target] += Consideration ? Consideration->Magnitude : 0.0f;
				Context.Stats[Target] = std::clamp(Context.Stats[Target], 0.0f, 1.0f);
				break;

			case EEffectType::Custom:
				if (CustomFn) CustomFn(Context);
				break;
			}
		}
	};

	struct UtAction 
	{
		std::string Name;
		std::vector<std::string> Tags;
		std::vector<UtEffect> Effects;
		std::unordered_map<std::string, UtConsideration> Considerations;

		void AddConsideration(const UtConsideration& NewCons)
		{
			auto It = Considerations.find(NewCons.Name);
			if (It != Considerations.end())
			{
				// Merge into existing consideration
				It->second.Weight *= NewCons.Weight;
				if (NewCons.Curve && NewCons.CurvePriority >= It->second.CurvePriority)
				{
					It->second.Curve = NewCons.Curve;
					It->second.CurvePriority = NewCons.CurvePriority;
				}
			}
			else
			{
				Considerations[NewCons.Name] = NewCons;
			}
		}

		// Generate considerations from effects
		void GenerateConsiderations() 
		{
			for (auto& Effect : Effects) 
			{
				if (Effect.Consideration) 
				{
					AddConsideration(Effect.Consideration.value());
				}
			}
		}

		// Weighted geometric mean (log-sum)
		float Score(const UtCharacterContext& Context) const 
		{
			if (Considerations.empty()) return 0.0f;

			float WeightedLogSum = 0.0f;
			float WeightSum = 0.0f;

			for (const auto& [Key, Consideration] : Considerations)
			{
				const float ConScore = std::clamp(Consideration.Score(Context), 0.001f, 1.0f); // avoid log(0)
				WeightedLogSum += Consideration.Weight * std::log(ConScore);
				WeightSum += Consideration.Weight;
			}

			return std::exp(WeightedLogSum / WeightSum);
		}

		void Execute(UtCharacterContext& Context) 
		{
			for (auto& Effect : Effects) 
			{
				Effect.Apply(Context);
			}
		}
	};

	// Biases add their effects and considerations to actions with the required tags
	struct UtBias
	{
		std::vector<std::string> RequiredTags;
		std::vector<UtEffect> Effects;
		std::vector<UtConsideration> Considerations;

		bool AppliesTo(const UtAction& Action) const 
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

	struct UtCharacterTrait
	{
		std::string Name;
		std::vector<UtBias> Biases;
	};

	inline void ApplyBiases(UtAction& Action, const std::vector<UtCharacterTrait>& Traits)
	{
		for (const auto& Trait : Traits)
		{
			for (const auto& Bias : Trait.Biases)
			{
				for (const auto& BiasCons : Bias.Considerations)
				{
					Action.AddConsideration(BiasCons);
				}
			}
		}
	}
}