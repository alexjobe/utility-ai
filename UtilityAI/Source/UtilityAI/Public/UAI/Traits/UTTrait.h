#pragma once
#include <Core/UTObject.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <UTAction.h>
#include <UTEffect.h>
#include <UTGoal.h>
#include <vector>

using namespace UTCore;

namespace UAI
{
struct UTBias
{
	std::string RequiredTag;
	float WeightMultiplier = 1.5f;
};

struct UTTrait: public UTObject
{
	std::unordered_set<std::string> OwnedTags;

	bool AppliesTo(const UTGoal& Goal) const;
	bool AppliesTo(const UTAction& Action) const;
	void ApplyToGoal(UTGoal& Goal) const;
	void ApplyToAction(UTAction& Action) const;

	bool AddEffect(const UTEffect& NewEffect);
	bool AddBias(const UTBias& NewBias);

	const std::vector<UTBias>& GetBiases() const { return Biases; }
	const std::unordered_map<std::string, UTEffect>& GetEffects() const { return Effects; }

private:
	std::vector<UTBias> Biases;
	std::unordered_map<std::string, UTEffect> Effects;
};
}