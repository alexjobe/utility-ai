#pragma once
#include "Math/Curves.h"
#include "Math/Math.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <string>
#include <unordered_map>

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

struct UTEvaluationData
{
	std::string Target; // e.g. "Wealth", "Survival"
	float Magnitude = 0.0f;
	float MinRaw = 0.0f;
	float MaxRaw = 1.0f;
	float Weight = 1.0f;
	int Priority = 0;
};

using ScoreFn = std::function<float(const UTAgentContext&, const UTEvaluationData&)>;

struct UTConsideration 
{
	std::string Key;
	UTEvaluationData Data;
	ScoreFn EvalRawScore = nullptr;
	CurveFn ScoreCurve = nullptr;

	float Score(const UTAgentContext& Context) const 
	{
		float RawScore = EvalRawScore 
			? EvalRawScore(Context, Data) 
			: Normalize(Data.Magnitude, Data.MinRaw, Data.MaxRaw);

		RawScore = std::clamp(RawScore, 0.f, 1.f);
		return ScoreCurve ? ScoreCurve(RawScore) : RawScore;
	}
};

inline float ScoreNeedChange(const UTAgentContext& Context, const UTEvaluationData& Data)
{
	const float Before = Context.GetNeed(Data.Target);

	const float After = std::clamp(Before - Data.Magnitude, Data.MinRaw, Data.MaxRaw);
	const float PercentChange = (After - Before) / (Data.MaxRaw - Data.MinRaw);

	const float BeforeNorm = Normalize(Before, Data.MinRaw, Data.MaxRaw);
	const float RawScore = BeforeNorm * -PercentChange; // Favor smaller values

	return std::clamp(RawScore, 0.0f, 1.0f);
}

inline void ApplyNeedChange(UTAgentContext& Context, const UTEvaluationData& Data)
{
	Context.Needs[Data.Target] -= Data.Magnitude;
	Context.Needs[Data.Target] = std::clamp(Context.Needs[Data.Target], Data.MinRaw, Data.MaxRaw);
}

class UTEffect
{
public:
	std::string Name;
	std::string ConsiderationKey;
	UTEvaluationData Data;
	ScoreFn EvalRawScore = nullptr;
	CurveFn ScoreCurve = nullptr;

	bool bIsConsideration = true;

	std::function<void(UTAgentContext&, const UTEvaluationData&)> EffectFn = nullptr;

	void Apply(UTAgentContext& Context) const
	{
		if(EffectFn)
		{
			EffectFn(Context, Data);
		}
		else
		{
			std::cout << "UTEffect: " + Name + " - EffectFn not set!" << std::endl;
		}
	}

	virtual UTConsideration AsConsideration() const
	{
		return { ConsiderationKey, Data, EvalRawScore, ScoreCurve };
	}
};
}