#pragma once
#include "Math/UTCurves.h"
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

using ScoreFnSig = float(const UTAgentContext&, const UTEvaluationData&);
using ScoreFnType = std::function<ScoreFnSig>;

struct UTConsideration
{
	std::string Key;
	UTEvaluationData Data;

	void SetRawScoreFnKey(const std::string& InKey);
	void SetScoreCurveFnKey(const std::string& InKey);
	std::string GetRawScoreFnKey() const { return RawScoreFnKey; }
	std::string GetScoreCurveFnKey() const { return ScoreCurveFnKey; }

	float Score(const UTAgentContext& Context) const;
	void DebugPrint();

private:
	std::string RawScoreFnKey;
	std::string ScoreCurveFnKey;
	const ScoreFnType* RawScoreFn = nullptr;
	const CurveFnType* ScoreCurveFn = nullptr;

	float EvalRawScore(const UTAgentContext& Context, const UTEvaluationData& Data) const;
	float EvalScoreCurve(float X) const;
};
}