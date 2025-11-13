#pragma once
#include <Core/UTObject.h>
#include <functional>
#include <string>
#include <unordered_map>
#include <UTAgentContext.h>
#include <UTConsideration.h>

using namespace UTCore;

namespace UAI
{
using PreconditionFnSig = bool(const UTAgentContext&);
using PreconditionFnType = std::function<PreconditionFnSig>;

class UTScorer
{
friend class UTAction;
friend class UTGoal;

public:
	bool AddConsideration(const UTConsideration& InNewCons);

	// Weighted geometric mean (log-sum)
	float Score(const UTAgentContext& Context) const;
	void SetOwner(const UTObject* InOwner) { Owner = InOwner; }

	void SetPreconditionFnKey(const std::string& InKey);
	std::string GetPreconditionFnKey() const { return PreconditionFnKey; }

	// Preconditions are quick "is this even possible?"
	bool PreconditionCheck(const UTAgentContext& InContext) const;

	std::vector<UTConsideration*> GetConsiderationsWithTag(const std::string& InTag);

private:
	const UTObject* Owner = nullptr;
	std::unordered_map<std::string, UTConsideration> Considerations;
	std::unordered_map<std::string, std::vector<std::string>> ConsTagIndex;

	std::string PreconditionFnKey;
	const PreconditionFnType* PreconditionFn = nullptr;
};
}
