#include <Logging/UTLogger.h>
#include <UTTrait.h>

using namespace UAI;

void UTTrait::ApplyToGoal(UTGoal& Goal) const
{
	for (auto& Bias : Biases)
	{
		std::vector<UTConsideration*> FoundCons = Goal.Scorer.GetConsiderationsByTarget(Bias.Target);
		for (UTConsideration* Cons : FoundCons)
		{
			Cons->AddBias(Bias);
		}
	}
}

void UTTrait::ApplyToAction(UTAction& Action) const
{
	for (auto& Bias : Biases)
	{
		std::vector<UTConsideration*> FoundCons = Action.Scorer.GetConsiderationsByTarget(Bias.Target);
		for (UTConsideration* Cons : FoundCons)
		{
			Cons->AddBias(Bias);
		}
	}

	/*for (const auto& [_, Effect] : Effects)
	{
		Action.AddEffect(Effect);
	}*/
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
	if (NewBias.Target.empty())
	{
		LOG_ERROR(std::format("[UTTrait] '{}' - Bias must have a target", GetKey()))
		return false;
	}

	Biases.push_back(NewBias);
	Biases.back().Source = GetName();

	return true;
}
