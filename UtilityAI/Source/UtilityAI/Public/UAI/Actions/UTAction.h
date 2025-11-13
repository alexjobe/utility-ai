#pragma once
#include <Core/UTObject.h>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <UTAgentContext.h>
#include <UTEffect.h>
#include <UTScorer.h>

using namespace UTCore;

namespace UAI
{
class UTAction : public UTObject
{
public:
	std::unordered_set<std::string> OwnedTags;
	UTScorer Scorer;

	UTAction();

	bool AddEffect(const UTEffect& NewEffect);

	// Generate considerations from effects
	void GenerateConsiderations();

	void Execute(UTAgentContext& Context);

	const std::unordered_map<std::string, UTConsideration>& GetConsiderations() const { return Scorer.Considerations; }
	const std::unordered_map<std::string, UTEffect>& GetEffects() const { return Effects; }

private:
	std::unordered_map<std::string, UTEffect> Effects;
};
}