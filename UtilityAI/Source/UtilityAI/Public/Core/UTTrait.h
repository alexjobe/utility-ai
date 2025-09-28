#pragma once
#include "UTAction.h"
#include "UTEffect.h"
#include "UTGoal.h"
#include "UTScorer.h"
#include <set>
#include <string>
#include <vector>

namespace UAI
{ 
// Biases add their effects and considerations to goals and actions with the required tags
struct UTBias
{
	std::set<std::string> RequiredTags;
	std::vector<UTEffect> Effects;
	std::vector<UTConsideration> Considerations;

	bool AppliesTo(const UTGoal& Goal) const
	{
		if (RequiredTags.empty()) return true;
		for (auto& Tag : RequiredTags)
		{
			if (Goal.RequiredTags.contains(Tag)) return true;
		}
		return false;
	}

	bool AppliesTo(const UTAction& Action) const
	{
		if (RequiredTags.empty()) return true;
		for (auto& Tag : RequiredTags)
		{
			if (Action.Tags.contains(Tag)) return true;
		}
		return false;
	}

	void ApplyToGoal(UTGoal& Goal) const
	{
		if (!AppliesTo(Goal)) return;
		for (const auto& Cons : Considerations)
		{
			Goal.Scorer.AddConsideration(Cons);
		}
	}

	void ApplyToAction(UTAction& Action) const
	{
		if (!AppliesTo(Action)) return;
		for (const auto& Cons : Considerations)
		{
			Action.Scorer.AddConsideration(Cons);
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