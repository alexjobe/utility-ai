#pragma once
#include "UTAgentContext.h"
#include "UTScorer.h"
#include <functional>
#include <set>
#include <string>

namespace UAI
{
using PreconditionFnSig = bool(const UTAgentContext&);
using PreconditionFnType = std::function<PreconditionFnSig>;

class UTGoal
{
public:
	std::set<std::string> Tags; // Only actions with the required tags will be considered
	UTScorer Scorer;

	UTGoal() = default;
	UTGoal(const std::string& InKey);

	std::string GetKey() const { return Key; }
	void SetKey(const std::string& InKey);

	void SetPreconditionFnKey(const std::string& InKey);
	std::string GetPreconditionFnKey() const { return PreconditionFnKey; }

	const std::unordered_map<std::string, UTConsideration>& GetConsiderations() const { return Scorer.Considerations; }

private:
	std::string Key;

	// Preconditions are quick "is this even possible?"
	std::string PreconditionFnKey;
	const PreconditionFnType* PreconditionFn = nullptr;
};
}