#pragma once
#include "UTAgentContext.h"
#include "UTConsideration.h"
#include <Core/UTObject.h>
#include <string>
#include <unordered_map>

using namespace UTCore;

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

	void SetOwner(const UTObject* InOwner) { Owner = InOwner; }

protected:
	const UTObject* Owner = nullptr;
	std::unordered_map<std::string, UTConsideration> Considerations;
};
}
