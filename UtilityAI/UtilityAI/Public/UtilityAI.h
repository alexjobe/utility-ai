#pragma once
#include <string>
#include <unordered_map>

namespace UtilityAI
{
struct UTAgentContext 
{
	std::unordered_map<std::string, float> Needs;
	std::unordered_map<std::string, float> Stats;

	float GetNeed(const std::string& Key) const
	{
		if (auto It = Needs.find(Key); It != Needs.end())
		{
			return It->second;
		}
		return 0.f;
	}

	float GetStat(const std::string& Key) const
	{
		if (auto It = Stats.find(Key); It != Stats.end())
		{
			return It->second;
		}
		return 0.f;
	}
};
}