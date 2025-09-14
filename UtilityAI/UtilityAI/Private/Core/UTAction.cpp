#include "Core/UTAction.h"
#include "Logging/Logger.h"
#include <cmath>
#include <format>
#include <string>

using namespace UtilityAI;

const bool UTAction::AddConsideration(const UTConsideration& NewCons)
{
	if (NewCons.Key.empty())
	{
		LOG_ERROR(std::format("Action: {} - Invalid Consideration! Check Key: {}", Name, NewCons.Key))
		return false;
	}

	auto It = Considerations.find(NewCons.Key);
	if (It != Considerations.end())
	{
		// Merge into existing consideration
		It->second.Data.Weight *= NewCons.Data.Weight;
		if (NewCons.ScoreCurve && NewCons.Data.Priority >= It->second.Data.Priority)
		{
			It->second.ScoreCurve = NewCons.ScoreCurve;
			It->second.Data.Priority = NewCons.Data.Priority;
		}
	}
	else
	{
		Considerations[NewCons.Key] = NewCons;
	}

	return true;
}

const bool UTAction::AddEffect(const UTEffect& NewEffect)
{
	if (NewEffect.Name.empty() || Effects.contains(NewEffect.Name))
	{
		LOG_ERROR(std::format("Action: {} - Invalid Effect! Check Key: {}", Name, NewEffect.Name))
		return false;
	}

	Effects[NewEffect.Name] = NewEffect;
	return true;
}

// Generate considerations from effects
void UTAction::GenerateConsiderations()
{
	LOG_INFO(std::format("Action: {} - Generating Considerations...", Name))

	for (auto& [Key, Effect] : Effects)
	{
		if (Effect.bIsConsideration && AddConsideration(Effect.AsConsideration()))
		{
			LOG_INFO(std::format("Effect: {} - Added Consideration: {}", Effect.Name, Effect.ConsiderationKey))
		}
	}
}

// Weighted geometric mean (log-sum)
float UTAction::Evaluate(const UTAgentContext& Context) const
{
	if (Considerations.empty())
	{
		LOG_WARN(std::format("Action: {} - No Considerations!", Name))
		return 0.0f;
	}

	LOG_INFO(std::format("Action: {} - Evaluating...", Name))

	float WeightedLogSum = 0.0f;
	float WeightSum = 0.0f;

	for (const auto& [Key, Consideration] : Considerations)
	{
		const float ConScore = std::clamp(Consideration.Score(Context), 0.001f, 1.0f); // avoid log(0)
		WeightedLogSum += Consideration.Data.Weight * std::log(ConScore);
		WeightSum += Consideration.Data.Weight;
		LOG_INFO(std::format("Action: {} - {} - Score: {} - Weight: {}", Name, Consideration.Key, ConScore, Consideration.Data.Weight))
	}

	const float FinalScore = std::exp(WeightedLogSum / WeightSum);
	LOG_INFO(std::format("Action: {} - Final Score: {}", Name, FinalScore))

	return FinalScore;
}

void UTAction::Execute(UTAgentContext& Context)
{
	for (auto& [Key, Effect] : Effects)
	{
		Effect.Apply(Context);
	}
}
