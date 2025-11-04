#include "Logging/UTLogger.h"
#include <format>
#include <string>
#include <UTAction.h>

using namespace UAI;

UTAction::UTAction()
{
	Scorer.SetOwner(this);
}

bool UTAction::AddEffect(const UTEffect& NewEffect)
{
	if (NewEffect.Key.empty() || Effects.contains(NewEffect.Key))
	{
		LOG_ERROR(std::format("[UTAction] '{}' - Invalid Effect! Check Key: '{}'", GetName(), NewEffect.Key))
		return false;
	}

	Effects[NewEffect.Key] = NewEffect;
	return true;
}

// Generate considerations from effects
void UTAction::GenerateConsiderations()
{
	LOG_INFO(std::format("[UTAction] '{}' - Generating Considerations...", GetName()))

	for (auto& [Key, Effect] : Effects)
	{
		if (Effect.bIsConsideration && Scorer.AddConsideration(Effect.AsConsideration()))
		{
			LOG_INFO(std::format("[UTAction] '{}' - '{}' added Consideration: '{}'", GetName(), Effect.Key, Effect.ConsiderationKey))
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
