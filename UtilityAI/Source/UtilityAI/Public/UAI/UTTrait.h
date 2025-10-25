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
// Traits add their effects and considerations to goals and actions with the required tags
struct UTTrait
{
	std::set<std::string> OwnedTags;
	std::set<std::string> RequiredTags;

	std::string GetKey() const { return Key; }
	void SetKey(const std::string& InKey) { Key = InKey; }

	bool AppliesTo(const UTGoal& Goal) const;
	bool AppliesTo(const UTAction& Action) const;
	void ApplyToGoal(UTGoal& Goal) const;
	void ApplyToAction(UTAction& Action) const;

	bool AddEffect(const UTEffect& NewEffect);
	bool AddConsideration(const UTConsideration& NewCons);

	const std::unordered_map<std::string, UTConsideration>& GetConsiderations() const { return Considerations; }
	const std::unordered_map<std::string, UTEffect>& GetEffects() const { return Effects; }

private:
	std::string Key;
	std::unordered_map<std::string, UTConsideration> Considerations;
	std::unordered_map<std::string, UTEffect> Effects;
};

inline void ApplyTraits(UTAction& Action, const std::vector<UTTrait>& Traits)
{
	for (const auto& Trait : Traits)
	{
		Trait.ApplyToAction(Action);
	}
}

inline void ApplyTraits(UTGoal& Goal, const std::vector<UTTrait>& Traits)
{
	for (const auto& Trait : Traits)
	{
		Trait.ApplyToGoal(Goal);
	}
}
}