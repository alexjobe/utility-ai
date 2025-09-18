#pragma once
#include "UTAgentContext.h"
#include "UTEffect.h"
#include "UTScorer.h"
#include <set>
#include <string>
#include <unordered_map>

namespace UAI
{
class UTAction
{
public:
	std::set<std::string> Tags;
	UTScorer Scorer;

	UTAction() = default;
	UTAction(const std::string& InKey);

	std::string GetKey() const { return Key; }
	void SetKey(const std::string& InKey) { Key = InKey; }

	const bool AddEffect(const UTEffect& NewEffect);

	// Generate considerations from effects
	void GenerateConsiderations();

	void Execute(UTAgentContext& Context);

private:
	std::string Key;
	std::unordered_map<std::string, UTEffect> Effects;
};
}