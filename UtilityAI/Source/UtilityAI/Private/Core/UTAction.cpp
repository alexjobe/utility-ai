#include "Core/UTAction.h"
#include "Logging/Logger.h"
#include <format>
#include <string>

using namespace UAI;

UTAction::UTAction(const std::string& InKey)
{
	SetKey(InKey);
}

void UTAction::SetKey(const std::string& InKey)
{
	Key = InKey;
	Scorer.SetOwnerKey(InKey);
}

bool UTAction::AddEffect(const UTEffect& NewEffect)
{
	if (NewEffect.Key.empty() || Effects.contains(NewEffect.Key))
	{
		LOG_ERROR(std::format("Action: {} - Invalid Effect! Check Key: {}", Key, NewEffect.Key))
		return false;
	}

	Effects[NewEffect.Key] = NewEffect;
	return true;
}

// Generate considerations from effects
void UTAction::GenerateConsiderations()
{
	LOG_INFO(std::format("Action: {} - Generating Considerations...", Key))

	for (auto& [Key, Effect] : Effects)
	{
		if (Effect.bIsConsideration && Scorer.AddConsideration(Effect.AsConsideration()))
		{
			LOG_INFO(std::format("Effect: {} - Added Consideration: {}", Effect.Key, Effect.ConsiderationKey))
		}
	}
}

void UTAction::Execute(UTAgentContext& Context)
{
	for (auto& [Key, Effect] : Effects)
	{
		Effect.Apply(Context);
	}
}
