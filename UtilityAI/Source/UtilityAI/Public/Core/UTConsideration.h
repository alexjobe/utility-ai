#pragma once
#include "Math/Curves.h"
#include "UTAgentContext.h"
#include <functional>
#include <string>

namespace UAI
{
struct UTEvaluationData
{
	std::string Target;
	float Raw = 0.0f;
	float MinRaw = 0.0f;
	float MaxRaw = 1.0f;
	float Weight = 1.0f;
	int Priority = 0;

	void DebugPrint();
};

using ScoreFn = std::function<float(const UTAgentContext&, const UTEvaluationData&)>;

struct UTConsideration
{
	std::string Key;
	UTEvaluationData Data;
	ScoreFn EvalRawScoreFn = nullptr;
	CurveFn ScoreCurveFn = nullptr;

	float Score(const UTAgentContext& Context) const;
	void DebugPrint();
};
}