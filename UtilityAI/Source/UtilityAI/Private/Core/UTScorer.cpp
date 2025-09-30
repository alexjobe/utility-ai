#include "Core/UTScorer.h"
#include "Logging/Logger.h"
#include <algorithm>
#include <cmath>
#include <string>

using namespace UAI;

bool UTScorer::AddConsideration(const UTConsideration& NewCons)
{
	if (NewCons.Key.empty())
	{
		LOG_ERROR(std::format("'{}' -- Invalid Consideration! Check Key: '{}'", OwnerKey, NewCons.Key))
		return false;
	}

	auto It = Considerations.find(NewCons.Key);
	if (It != Considerations.end())
	{
		// Merge into existing consideration
		It->second.Data.Weight *= NewCons.Data.Weight;
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
	if (Considerations.empty())
	{
		LOG_WARN(std::format("'{}' -- No Considerations!", OwnerKey))
		return 0.0f;
	}

	LOG_INFO(std::format("'{}' -- Evaluating...", OwnerKey))

	float WeightedLogSum = 0.0f;
	float WeightSum = 0.0f;

	for (const auto& [Key, Consideration] : Considerations)
	{
		const float ConScore = std::clamp(Consideration.Score(Context), 0.001f, 1.0f); // avoid log(0)
		WeightedLogSum += Consideration.Data.Weight * std::log(ConScore);
		WeightSum += Consideration.Data.Weight;
		LOG_INFO(std::format("'{}' -- ['{}' | Score: {} | Weight: {}]", OwnerKey, Consideration.Key, ConScore, Consideration.Data.Weight))
	}

	const float FinalScore = std::exp(WeightedLogSum / WeightSum);
	LOG_INFO(std::format("'{}' -- Final Score: {}", OwnerKey, FinalScore))

	return FinalScore;
}

