#include "UAI/UTTrait.h"
#include <Logging/UTLogger.h>

using namespace UAI;

bool UTTrait::AppliesTo(const UTGoal& Goal) const
{
	if (Tags.empty()) return true;
	for (auto& Tag : Tags)
	{
		if (Goal.Tags.contains(Tag)) return true;
	}
	return false;
}

bool UTTrait::AppliesTo(const UTAction& Action) const
{
	if (Tags.empty()) return true;
	for (auto& Tag : Tags)
	{
		if (Action.Tags.contains(Tag)) return true;
	}
	return false;
}

void UTTrait::ApplyToGoal(UTGoal& Goal) const
{
	if (!AppliesTo(Goal)) return;
	for (const auto& [_, Cons] : Considerations)
	{
		Goal.Scorer.AddConsideration(Cons);
	}
}

void UTTrait::ApplyToAction(UTAction& Action) const
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

bool UTTrait::AddEffect(const UTEffect& NewEffect)
{
	if (NewEffect.Key.empty() || Effects.contains(NewEffect.Key))
	{
		LOG_ERROR(std::format("Trait: '{}' - Invalid Effect! Check Key: '{}'", Key, NewEffect.Key))
		return false;
	}
	Effects[NewEffect.Key] = NewEffect;
	return true;
}

bool UTTrait::AddConsideration(const UTConsideration& NewCons)
{
	if (NewCons.Key.empty() || Considerations.contains(NewCons.Key))
	{
		LOG_ERROR(std::format("Trait: '{}' - Invalid Consideration! Check Key: '{}'", Key, NewCons.Key))
		return false;
	}
	Considerations[NewCons.Key] = NewCons;
	return true;
}
