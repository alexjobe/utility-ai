#pragma once
#include "UtilityAI.h"
#include <functional>
#include <set>
#include <string>

namespace UtilityAI
{
class UTGoal
{
public:
	std::string Name;
	std::set<std::string> RequiredTags; // Only actions with the required tags will be considered

	const bool AddConsideration(const UTConsideration& NewCons);

	// Weighted geometric mean (log-sum)
	float Evaluate(const UTAgentContext& Context) const;

	// Preconditions are quick "is this even possible?"
	std::function<bool(const UTAgentContext&)> PreconditionFn = nullptr;
private:
	std::unordered_map<std::string, UTConsideration> Considerations;
};
}