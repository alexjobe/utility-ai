#include "Core/UTAction.h"
#include "Logging/Logger.h"
#include <format>
#include <string>

using namespace UtilityAI;

UTAction::UTAction(const std::string& InKey)
{
	Key = InKey;
	Scorer.SetOwnerKey(InKey);
}

const bool UTAction::AddEffect(const UTEffect& NewEffect)
{
	if (NewEffect.Name.empty() || Effects.contains(NewEffect.Name))
	{
		LOG_ERROR(std::format("Action: {} - Invalid Effect! Check Key: {}", Key, NewEffect.Name))
		return false;
	}

	Effects[NewEffect.Name] = NewEffect;
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
			LOG_INFO(std::format("Effect: {} - Added Consideration: {}", Effect.Name, Effect.ConsiderationKey))
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
