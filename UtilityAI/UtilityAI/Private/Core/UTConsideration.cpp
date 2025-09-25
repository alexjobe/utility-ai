#include "Logging/Logger.h"
#include "Math/Math.h"
#include <algorithm>
#include <Core/UTConsideration.h>
#include <format>

using namespace UAI;

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
	float RawScore = EvalRawScoreFn
		? EvalRawScoreFn(Context, Data)
		: Normalize(Data.Raw, Data.MinRaw, Data.MaxRaw);

	RawScore = std::clamp(RawScore, 0.f, 1.f);
	return ScoreCurveFn ? ScoreCurveFn(RawScore) : RawScore;
}

void UTConsideration::DebugPrint()
{
	LOG_INFO("-- Consideration --")
	LOG_INFO(std::format("Key: {}", Key))
	LOG_INFO(std::format("EvalRawScoreFn: {}", EvalRawScoreFn.target_type().name()))
	LOG_INFO(std::format("ScoreCurveFn: {}", ScoreCurveFn.target_type().name()))
	Data.DebugPrint();
}
