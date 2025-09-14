#pragma once
#include "UTConsideration.h"
#include "UtilityAI.h"
#include <string>
#include <unordered_map>

namespace UtilityAI
{
class UTScorer
{
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
