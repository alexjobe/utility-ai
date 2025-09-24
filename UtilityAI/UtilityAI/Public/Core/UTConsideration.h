#pragma once
#include "Math/Curves.h"
#include "Math/Math.h"
#include "UTAgentContext.h"
#include <algorithm>
#include <functional>
#include <string>

namespace UAI
{
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
	ScoreFn EvalRawScoreFn = nullptr;
	CurveFn ScoreCurveFn = nullptr;

	float Score(const UTAgentContext& Context) const
	{
		float RawScore = EvalRawScoreFn
			? EvalRawScoreFn(Context, Data)
			: Normalize(Data.Raw, Data.MinRaw, Data.MaxRaw);

		RawScore = std::clamp(RawScore, 0.f, 1.f);
		return ScoreCurveFn ? ScoreCurveFn(RawScore) : RawScore;
	}
};
}