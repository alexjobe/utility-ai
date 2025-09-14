#pragma once
#include "Math/Curves.h"
#include "Math/Math.h"
#include <algorithm>
#include <functional>
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
	float Raw = 0.0f;
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
			: Normalize(Data.Raw, Data.MinRaw, Data.MaxRaw);

		RawScore = std::clamp(RawScore, 0.f, 1.f);
		return ScoreCurve ? ScoreCurve(RawScore) : RawScore;
	}
};
}