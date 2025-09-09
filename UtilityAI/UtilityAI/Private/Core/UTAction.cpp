#include "Core/UTAction.h"
#include <cmath>
#include <iostream>

using namespace UtilityAI;

const UTConsideration& UTAction::AddConsideration(const UTConsideration& NewCons)
{
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

	return Considerations[NewCons.Key];
}

const void UTAction::AddEffect(std::unique_ptr<UTEffect> NewEffect)
{
	if (NewEffect)
	{
		Effects[NewEffect->Name] = std::move(NewEffect);
	}
}

// Generate considerations from effects
void UTAction::GenerateConsiderations()
{
	std::cout << "Action: " + Name + " - Generating Considerations..." << std::endl;
	for (auto& [Key, Effect] : Effects)
	{
		if (Effect && Effect->bIsConsideration)
		{
			const UTConsideration& NewCons = AddConsideration(Effect->AsConsideration());
			std::cout << "Effect " + Effect->Name + " added Consideration " + NewCons.Key << std::endl;
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
		if (Effect) Effect->Apply(Context);
	}
}
