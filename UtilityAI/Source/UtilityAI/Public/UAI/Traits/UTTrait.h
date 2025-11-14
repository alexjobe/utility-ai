#pragma once
#include <Core/UTObject.h>
#include <string>
#include <unordered_map>
#include <UTAction.h>
#include <UTConsideration.h>
#include <UTEffect.h>
#include <UTGoal.h>
#include <vector>

using namespace UTCore;

namespace UAI
{
struct UTTrait: public UTObject
{
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