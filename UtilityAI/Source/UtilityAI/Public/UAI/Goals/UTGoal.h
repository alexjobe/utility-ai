#pragma once
#include <Core/UTObject.h>
#include <set>
#include <string>
#include <UTScorer.h>

using namespace UTCore;

namespace UAI
{
class UTGoal : public UTObject
{
public:
	std::set<std::string> OwnedTags;
	std::set<std::string> RequiredTags; // Only actions with the required tags will be considered
	UTScorer Scorer;

	UTGoal();

	const std::unordered_map<std::string, UTConsideration>& GetConsiderations() const { return Scorer.Considerations; }
};
}