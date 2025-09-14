#pragma once
#include "UTAction.h"
#include "UtilityAI.h"
#include <set>
#include <string>
#include <vector>

namespace UtilityAI
{ 
// Biases add their effects and considerations to actions with the required tags
struct UTBias
{
	std::set<std::string> RequiredTags;
	std::vector<UTEffect> Effects;
	std::vector<UTConsideration> Considerations;

	bool AppliesTo(const UTAction& Action) const
	{
		if (RequiredTags.empty()) return true;
		for (auto& Tag : RequiredTags)
		{
			if (std::find(Action.Tags.begin(), Action.Tags.end(), Tag) != Action.Tags.end())
			{
				return true;
			}
		}
		return false;
	}

	void ApplyToAction(UTAction& Action) const
	{
		if (!AppliesTo(Action)) return;
		
		for (const auto& Cons : Considerations)
		{
			Action.AddConsideration(Cons);
		}

		for (const auto& Effect : Effects)
		{
			Action.AddEffect(Effect);
		}
	}
};

struct UTTrait
{
	std::string Name;
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
}