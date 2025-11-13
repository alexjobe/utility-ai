#include <Logging/UTLogger.h>
#include <UTTrait.h>

using namespace UAI;

bool UTTrait::AppliesTo(const UTGoal& Goal) const
{
	for (auto& Bias : Biases)
	{
		if (Goal.OwnedTags.contains(Bias.RequiredTag)) return true;
	}
	return false;
}

bool UTTrait::AppliesTo(const UTAction& Action) const
{
	for (auto& Bias : Biases)
	{
		if (Action.OwnedTags.contains(Bias.RequiredTag)) return true;
	}
	return false;
}

void UTTrait::ApplyToGoal(UTGoal& Goal) const
{
	if (!AppliesTo(Goal)) return;
	for (auto& Bias : Biases)
	{
		std::vector<UTConsideration*> FoundCons = Goal.Scorer.GetConsiderationsWithTag(Bias.RequiredTag);
		for (UTConsideration* Cons : FoundCons)
		{
			Cons->Data.Weight *= Bias.WeightMultiplier;
		}
	}
}

void UTTrait::ApplyToAction(UTAction& Action) const
{
	if (!AppliesTo(Action)) return;
	for (auto& Bias : Biases)
	{
		std::vector<UTConsideration*> FoundCons = Action.Scorer.GetConsiderationsWithTag(Bias.RequiredTag);
		for (UTConsideration* Cons : FoundCons)
		{
			Cons->Data.Weight *= Bias.WeightMultiplier;
		}
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
		LOG_ERROR(std::format("[UTTrait] '{}' - Invalid Effect! Check Key: '{}'", GetKey(), NewEffect.Key))
		return false;
	}
	Effects[NewEffect.Key] = NewEffect;
	return true;
}

bool UTTrait::AddBias(const UTBias& NewBias)
{
	if (NewBias.RequiredTag.empty())
	{
		LOG_ERROR(std::format("[UTTrait] '{}' - Bias must have a required tag", GetKey()))
		return false;
	}

	Biases.push_back(NewBias);
	return true;
}
