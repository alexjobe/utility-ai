#include "Logging/UTLogger.h"
#include <algorithm>
#include <cmath>
#include <string>
#include <UTFunctionRegistry.h>
#include <UTScorer.h>

using namespace UAI;

bool UTScorer::AddConsideration(const UTConsideration& InNewCons)
{
	if (Owner == nullptr)
	{
		LOG_ERROR("[UTScorer] Owner not set!")
		return false;
	}

	if (InNewCons.Key.empty())
	{
		LOG_ERROR(std::format("[UTScorer] '{}' -- Invalid Consideration! Check Key: '{}'", Owner->GetName(), InNewCons.Key))
		return false;
	}

	auto It = Considerations.find(InNewCons.Key);
	if (It != Considerations.end())
	{
		LOG_ERROR(std::format("[UTScorer] '{}' -- Cannot add duplicate Consideration: '{}'", Owner->GetName(), InNewCons.Key))
		return false;
	}

	Considerations[InNewCons.Key] = InNewCons;

	// Build TagIndex for fast filtering
	for (const auto& Tag : InNewCons.OwnedTags)
	{
		ConsTagIndex[Tag].push_back(InNewCons.Key);
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

void UTScorer::SetPreconditionFnKey(const std::string& InKey)
{
	PreconditionFn = UTFunctionRegistry::Instance().Get<PreconditionFnSig>(InKey);
	PreconditionFnKey = PreconditionFn ? InKey : "";
}

bool UTScorer::PreconditionCheck(const UTAgentContext& InContext) const
{
	if (PreconditionFn && *PreconditionFn)
	{
		return (*PreconditionFn)(InContext);
	}
	return true;
}

std::vector<UTConsideration*> UTScorer::GetConsiderationsWithTag(const std::string& InTag)
{
	std::vector<UTConsideration*> Result;

	auto TagIt = ConsTagIndex.find(InTag);
	if(TagIt == ConsTagIndex.end()) return Result;

	for (const auto& ConsKey : TagIt->second)
	{
		if (auto ConsIt = Considerations.find(ConsKey); ConsIt != Considerations.end())
		{
			Result.push_back(&ConsIt->second);
		}
		else
		{
			LOG_ERROR(std::format("[UTScorer] '{}' -- Tag '{}' maps to invalid key '{}'!", Owner->GetName(), InTag, ConsKey))
		}
	}

	return Result;
}
