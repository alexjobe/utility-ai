#pragma once
#include "UTEffect.h"
#include "UtilityAI.h"
#include "UTScorer.h"
#include <set>
#include <string>
#include <unordered_map>

namespace UtilityAI
{
class UTAction
{
public:
	std::set<std::string> Tags;
	UTScorer Scorer;

	UTAction(const std::string& InKey);

	std::string GetKey() const { return Key; }

	const bool AddEffect(const UTEffect& NewEffect);

	// Generate considerations from effects
	void GenerateConsiderations();

	void Execute(UTAgentContext& Context);

private:
	std::string Key;
	std::unordered_map<std::string, UTEffect> Effects;
};
}