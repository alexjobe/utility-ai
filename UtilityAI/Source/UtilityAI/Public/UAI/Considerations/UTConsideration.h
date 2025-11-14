#pragma once
#include "Math/UTCurves.h"
#include "UTAgentContext.h"
#include <functional>
#include <string>
#include <vector>

namespace UAI
{
struct UTEvaluationData
{
	std::string Target;
	float Raw = 0.0f;
	float MinRaw = 0.0f;
	float MaxRaw = 1.0f;
	float BaseWeight = 1.0f;
	int Priority = 0;

	void DebugPrint();
};

struct UTBias
{
	std::string Source;
	std::string Target;
	float WeightMultiplier = 1.5f;
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

	void AddBias(const UTBias& InBias);
	const std::vector<UTBias>& GetBiases() const { return Biases; }
	float GetBiasedWeight() const;

	float Score(const UTAgentContext& Context) const;
	void DebugPrint();

private:
	std::string RawScoreFnKey;
	std::string ScoreCurveFnKey;
	const ScoreFnType* RawScoreFn = nullptr;
	const CurveFnType* ScoreCurveFn = nullptr;

	std::vector<UTBias> Biases;
	float BiasedWeight = 1.f;

	float EvalRawScore(const UTAgentContext& Context, const UTEvaluationData& Data) const;
	float EvalScoreCurve(float X) const;
};
}