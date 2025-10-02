#pragma once
#include "UTAgentContext.h"
#include "UTConsideration.h"
#include <string>
#include <unordered_map>

namespace UAI
{
class UTScorer
{
friend class UTAction;
friend class UTGoal;

public:
	bool AddConsideration(const UTConsideration& NewCons);

	// Weighted geometric mean (log-sum)
	float Score(const UTAgentContext& Context) const;

	void SetOwnerKey(const std::string& Key) { OwnerKey = Key; }

protected:
	std::string OwnerKey;
	std::unordered_map<std::string, UTConsideration> Considerations;
};
}
