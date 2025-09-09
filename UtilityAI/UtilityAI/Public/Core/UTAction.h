#pragma once
#include <memory>
#include <string>
#include <unordered_map>
#include <UtilityAI.h>
#include <vector>

namespace UtilityAI
{
class UTAction
{
public:
	std::string Name;
	std::vector<std::string> Tags;

	const UTConsideration& AddConsideration(const UTConsideration& NewCons);
	const void AddEffect(std::unique_ptr<UTEffect> NewEffect);

	// Generate considerations from effects
	void GenerateConsiderations();

	// Weighted geometric mean (log-sum)
	float Score(const UTAgentContext& Context) const;

	void Execute(UTAgentContext& Context);

private:
	std::unordered_map<std::string, std::unique_ptr<UTEffect>> Effects;
	std::unordered_map<std::string, UTConsideration> Considerations;
};
}