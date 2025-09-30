#include "Core/UTFunctionRegistry.h"
#include "Logging/Logger.h"
#include "Math/Curves.h"
#include "Math/Math.h"
#include <algorithm>
#include <Core/UTConsideration.h>
#include <format>

using namespace UAI;

void UTConsideration::SetRawScoreFnKey(const std::string& InKey)
{
	RawScoreFnKey = InKey;
	if (!InKey.empty())
	{
		RawScoreFn = UTFunctionRegistry::Instance().Get<ScoreFnSig>(InKey);
	}
	else
	{
		RawScoreFn = nullptr;
	}
}

void UTConsideration::SetScoreCurveFnKey(const std::string& InKey)
{
	ScoreCurveFnKey = InKey;
	if (!InKey.empty())
	{
		ScoreCurveFn = UTFunctionRegistry::Instance().Get<CurveFnSig>(InKey);
	}
	else
	{
		ScoreCurveFn = nullptr;
	}
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
		: Normalize(Data.Raw, Data.MinRaw, Data.MaxRaw);

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
		return std::clamp(RawScore, 0.f, 1.f);
	}
	return 0.0f;
}

float UTConsideration::EvalScoreCurve(float X) const
{
	if (ScoreCurveFn && *ScoreCurveFn) 
	{
		const float CurveScore = (*ScoreCurveFn)(X);
		return std::clamp(CurveScore, 0.f, 1.f);
	}
	return 0.0f;
}
