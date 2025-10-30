#pragma once
#include <string>
#include <unordered_map>

namespace UAI
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

	void SetNeed(const std::string& Key, float Value) { Needs[Key] = Value; }
	void SetStat(const std::string& Key, float Value) { Stats[Key] = Value; }
};
}