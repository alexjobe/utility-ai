#include "Core/UTTrait.h"
#include <Logging/Logger.h>

using namespace UAI;

bool UTBias::AppliesTo(const UTGoal& Goal) const
{
	if (Tags.empty()) return true;
	for (auto& Tag : Tags)
	{
		if (Goal.RequiredTags.contains(Tag)) return true;
	}
	return false;
}

bool UTBias::AppliesTo(const UTAction& Action) const
{
	if (Tags.empty()) return true;
	for (auto& Tag : Tags)
	{
		if (Action.Tags.contains(Tag)) return true;
	}
	return false;
}

void UTBias::ApplyToGoal(UTGoal& Goal) const
{
	if (!AppliesTo(Goal)) return;
	for (const auto& [_, Cons] : Considerations)
	{
		Goal.Scorer.AddConsideration(Cons);
	}
}

void UTBias::ApplyToAction(UTAction& Action) const
{
	if (!AppliesTo(Action)) return;
	for (const auto& [_, Cons] : Considerations)
	{
		Action.Scorer.AddConsideration(Cons);
	}
	for (const auto& [_, Effect] : Effects)
	{
		Action.AddEffect(Effect);
	}
}

bool UTBias::AddEffect(const UTEffect& NewEffect)
{
	if (NewEffect.Key.empty() || Effects.contains(NewEffect.Key))
	{
		LOG_ERROR(std::format("Bias - Invalid Effect! Check Key: {}", NewEffect.Key))
		return false;
	}
	Effects[NewEffect.Key] = NewEffect;
	return true;
}

bool UTBias::AddConsideration(const UTConsideration& NewCons)
{
	if (NewCons.Key.empty() || Considerations.contains(NewCons.Key))
	{
		LOG_ERROR(std::format("Bias - Invalid Consideration! Check Key: {}", NewCons.Key))
		return false;
	}
	Considerations[NewCons.Key] = NewCons;
	return true;
}
