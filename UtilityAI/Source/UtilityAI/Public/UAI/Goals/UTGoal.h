#pragma once
#include <Core/UTObject.h>
#include <string>
#include <unordered_set>
#include <UTScorer.h>

using namespace UTCore;

namespace UAI
{
class UTGoal : public UTObject
{
public:
	std::unordered_set<std::string> RequiredTags; // Only actions with the required tags will be considered
	UTScorer Scorer;

	UTGoal();

	const std::unordered_map<std::string, UTConsideration>& GetConsiderations() const { return Scorer.Considerations; }
};
}