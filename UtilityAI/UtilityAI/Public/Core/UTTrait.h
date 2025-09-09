#pragma once
#include "UTAction.h"
#include <string>
#include <UtilityAI.h>
#include <vector>

namespace UtilityAI
{ 
// Biases add their effects and considerations to actions with the required tags
struct UTBias
{
	std::vector<std::string> RequiredTags;
	std::vector<UTEffect> Effects;
	std::vector<UTConsideration> Considerations;

	bool AppliesTo(const UTAction& Action) const
	{
		if (RequiredTags.empty()) return true;
		for (auto& Tag : RequiredTags)
		{
			if (std::find(Action.Tags.begin(), Action.Tags.end(), Tag) != Action.Tags.end())
				return true;
		}
		return false;
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
			for (const auto& BiasCons : Bias.Considerations)
			{
				Action.AddConsideration(BiasCons);
			}

			for (const auto& BiasEffect : Bias.Effects)
			{
				Action.AddEffect(BiasEffect);
			}
		}
	}
}
}