#pragma once
#include "UtilityAI.h"
#include "UTScorer.h"
#include <functional>
#include <set>
#include <string>

namespace UtilityAI
{
class UTGoal
{
public:
	std::set<std::string> RequiredTags; // Only actions with the required tags will be considered
	UTScorer Scorer;

	UTGoal(const std::string& InKey);

	// Preconditions are quick "is this even possible?"
	std::function<bool(const UTAgentContext&)> PreconditionFn = nullptr;

	std::string GetKey() const { return Key; }

private:
	std::string Key;
};
}