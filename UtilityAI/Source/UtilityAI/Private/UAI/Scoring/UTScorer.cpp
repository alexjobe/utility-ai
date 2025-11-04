#include "Logging/UTLogger.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <UTScorer.h>

using namespace UAI;

bool UTScorer::AddConsideration(const UTConsideration& NewCons)
{
	if (Owner == nullptr)
	{
		LOG_ERROR("[UTScorer] Owner not set!")
		return false;
	}

	if (NewCons.Key.empty())
	{
		LOG_ERROR(std::format("[UTScorer] '{}' -- Invalid Consideration! Check Key: '{}'", Owner->GetName(), NewCons.Key))
		return false;
	}

	auto It = Considerations.find(NewCons.Key);
	if (It != Considerations.end())
	{
		// Merge into existing consideration
		It->second.Data.Weight = (It->second.Data.Weight + NewCons.Data.Weight) * 0.5f; // Average
		const std::string ScoreCurveFnKey = NewCons.GetScoreCurveFnKey();
		if (!ScoreCurveFnKey.empty() && NewCons.Data.Priority >= It->second.Data.Priority)
		{
			It->second.SetScoreCurveFnKey(ScoreCurveFnKey);
			It->second.Data.Priority = NewCons.Data.Priority;
		}
	}
	else
	{
		Considerations[NewCons.Key] = NewCons;
	}

	return true;
}

// Weighted geometric mean (log-sum)
float UTScorer::Score(const UTAgentContext& Context) const
{
	if (Owner == nullptr)
	{
		LOG_ERROR("[UTScorer] Owner not set!")
		return 0.0f;
	}

	if (Considerations.empty())
	{
		LOG_WARN(std::format("[UTScorer] '{}' -- No Considerations!", Owner->GetName()))
		return 0.0f;
	}

	LOG_INFO(std::format("[UTScorer] '{}' -- Evaluating...", Owner->GetName()))

	float WeightSum = 0.0f;
	for (const auto& [_, Cons] : Considerations)
	{
		if (Cons.Data.Weight < 0.0f)
		{
			LOG_WARN(std::format("[UTScorer] '{}' -- Negative weight on '{}'; clamping to zero.", Owner->GetName(), Cons.Key));
		}

		WeightSum += std::max(0.0f, Cons.Data.Weight);
	}

	if (WeightSum <= 0.0f)
	{
		LOG_WARN(std::format("[UTScorer] '{}' -- All weights are zero or invalid.", Owner->GetName()));
		return 0.0f;
	}

	float WeightedLogSum = 0.0f;
	for (const auto& [Key, Consideration] : Considerations)
	{
		const float ConScore = std::clamp(Consideration.Score(Context), 0.001f, 1.0f); // avoid log(0)
		const float WeightedLog = Consideration.Data.Weight * std::log(ConScore);
		WeightedLogSum += WeightedLog;

		const float Contribution = std::pow(ConScore, Consideration.Data.Weight / WeightSum);
		LOG_INFO(std::format("[UTScorer] '{}' -- ['{}' | Score: {} | Weight: {} | Contrib: {}]", Owner->GetName(), Consideration.Key, ConScore, Consideration.Data.Weight, Contribution))
	}

	const float FinalScore = std::exp(WeightedLogSum / WeightSum);
	LOG_INFO(std::format("[UTScorer] '{}' -- Final Score: {}", Owner->GetName(), FinalScore))

	return FinalScore;
}

