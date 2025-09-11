#include "Core/UTAction.h"
#include <cmath>
#include <format>
#include <Logging/Logger.h>
#include <string>

using namespace UtilityAI;

const bool UTAction::AddConsideration(const UTConsideration& NewCons)
{
	if (NewCons.Key.empty())
	{
		LOG_ERROR(std::format("Invalid Consideration! Check Key: {}", NewCons.Key))
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
		LOG_ERROR(std::format("Invalid Effect! Check Key: {}", NewEffect.Name))
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
float UTAction::Score(const UTAgentContext& Context) const
{
	if (Considerations.empty()) return 0.0f;

	float WeightedLogSum = 0.0f;
	float WeightSum = 0.0f;

	for (const auto& [Key, Consideration] : Considerations)
	{
		const float ConScore = std::clamp(Consideration.Score(Context), 0.001f, 1.0f); // avoid log(0)
		WeightedLogSum += Consideration.Data.Weight * std::log(ConScore);
		WeightSum += Consideration.Data.Weight;
	}

	return std::exp(WeightedLogSum / WeightSum);
}

void UTAction::Execute(UTAgentContext& Context)
{
	for (auto& [Key, Effect] : Effects)
	{
		Effect.Apply(Context);
	}
}
