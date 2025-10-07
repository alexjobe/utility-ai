#pragma once
#include "UTAction.h"
#include "UTEffect.h"
#include "UTGoal.h"
#include <set>
#include <string>
#include <unordered_map>
#include <vector>

namespace UAI
{ 
// Biases add their effects and considerations to goals and actions with the required tags
struct UTBias
{
	std::set<std::string> Tags;

	bool AppliesTo(const UTGoal& Goal) const;
	bool AppliesTo(const UTAction& Action) const;
	void ApplyToGoal(UTGoal& Goal) const;
	void ApplyToAction(UTAction& Action) const;

	bool AddEffect(const UTEffect& NewEffect);
	bool AddConsideration(const UTConsideration& NewCons);

	const std::unordered_map<std::string, UTConsideration>& GetConsiderations() const { return Considerations; }
	const std::unordered_map<std::string, UTEffect>& GetEffects() const { return Effects; }

private:
	std::unordered_map<std::string, UTConsideration> Considerations;
	std::unordered_map<std::string, UTEffect> Effects;
};

struct UTTrait
{
	std::string Key;
	std::vector<UTBias> Biases;
};

inline void ApplyBiases(UTAction& Action, const std::vector<UTTrait>& Traits)
{
	for (const auto& Trait : Traits)
	{
		for (const auto& Bias : Trait.Biases)
		{
			Bias.ApplyToAction(Action);
		}
	}
}

inline void ApplyBiases(UTGoal& Goal, const std::vector<UTTrait>& Traits)
{
	for (const auto& Trait : Traits)
	{
		for (const auto& Bias : Trait.Biases)
		{
			Bias.ApplyToGoal(Goal);
		}
	}
}
}