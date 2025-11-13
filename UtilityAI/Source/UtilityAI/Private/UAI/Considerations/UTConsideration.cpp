#include "Logging/UTLogger.h"
#include "Math/UTCurves.h"
#include "Math/UTMath.h"
#include "UTFunctionRegistry.h"
#include <algorithm>
#include <format>
#include <UTConsideration.h>

using namespace UAI;

void UTConsideration::SetRawScoreFnKey(const std::string& InKey)
{
	RawScoreFn = UTFunctionRegistry::Instance().Get<ScoreFnSig>(InKey);
	RawScoreFnKey = RawScoreFn ? InKey : "";
}

void UTConsideration::SetScoreCurveFnKey(const std::string& InKey)
{
	ScoreCurveFn = UTFunctionRegistry::Instance().Get<CurveFnSig>(InKey);
	ScoreCurveFnKey = ScoreCurveFn ? InKey : "";
}

void UTEvaluationData::DebugPrint()
{
	LOG_INFO("-- EvaluationData --")
	LOG_INFO(std::format("Target: {}", Target))
	LOG_INFO(std::format("Raw: {}", Raw))
	LOG_INFO(std::format("MinRaw: {}", MinRaw))
	LOG_INFO(std::format("MaxRaw: {}", MaxRaw))
	LOG_INFO(std::format("Weight: {}", Weight))
	LOG_INFO(std::format("Priority: {}", Priority))
}

float UTConsideration::Score(const UTAgentContext& Context) const
{
	float RawScore = RawScoreFn
		? EvalRawScore(Context, Data)
		: UTMath::Normalize(Data.Raw, Data.MinRaw, Data.MaxRaw);

	return ScoreCurveFn ? EvalScoreCurve(RawScore) : RawScore;
}

void UTConsideration::DebugPrint()
{
	LOG_INFO("-- Consideration --")
	LOG_INFO(std::format("Key: {}", Key))
	LOG_INFO(std::format("RawScoreFn: {}", RawScoreFnKey))
	LOG_INFO(std::format("ScoreCurveFn: {}", ScoreCurveFnKey))
	Data.DebugPrint();
}

float UTConsideration::EvalRawScore(const UTAgentContext& Context, const UTEvaluationData& Data) const
{
	if (RawScoreFn && *RawScoreFn) 
	{
		const float RawScore = (*RawScoreFn)(Context, Data);

		if (RawScore < 0 || RawScore > 1)
		{
			LOG_WARN(std::format("[UTConsideration] (Consideration: {}) RawScoreFn should return a normalized value. Result: {}", Key, RawScore))
		}

		return std::clamp(RawScore, 0.f, 1.f);
	}
	return 0.0f;
}

float UTConsideration::EvalScoreCurve(float X) const
{
	if (ScoreCurveFn && *ScoreCurveFn) 
	{
		const float CurveScore = (*ScoreCurveFn)(X);

		if (CurveScore < 0 || CurveScore > 1)
		{
			LOG_WARN(std::format("[UTConsideration] (Consideration: {}) ScoreCurveFn should return a normalized value. Result: {}", Key, CurveScore))
		}

		return std::clamp(CurveScore, 0.f, 1.f);
	}
	return 0.0f;
}
