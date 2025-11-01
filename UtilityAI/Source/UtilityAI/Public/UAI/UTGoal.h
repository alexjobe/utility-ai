#pragma once
#include "UTAgentContext.h"
#include "UTScorer.h"
#include <Core/UTObject.h>
#include <functional>
#include <set>
#include <string>

using namespace UTCore;

namespace UAI
{
using PreconditionFnSig = bool(const UTAgentContext&);
using PreconditionFnType = std::function<PreconditionFnSig>;

class UTGoal : public UTObject
{
public:
	std::set<std::string> OwnedTags;
	std::set<std::string> RequiredTags; // Only actions with the required tags will be considered
	UTScorer Scorer;

	UTGoal();

	void SetPreconditionFnKey(const std::string& InKey);
	std::string GetPreconditionFnKey() const { return PreconditionFnKey; }
	bool PreconditionCheck(const UTAgentContext& InContext) const;

	const std::unordered_map<std::string, UTConsideration>& GetConsiderations() const { return Scorer.Considerations; }

private:
	// Preconditions are quick "is this even possible?"
	std::string PreconditionFnKey;
	const PreconditionFnType* PreconditionFn = nullptr;
};
}