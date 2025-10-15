#pragma once
#include "Core/UTAgentContext.h"
#include "StatTypes.h"
#include <string>
#include <unordered_map>
#include <unordered_set>

using namespace UAI;

namespace Game
{
	class GCharacter
	{
	public:
		GCharacter() = default;
		GCharacter(const std::string& InKey);
		std::string Key;
		std::unordered_set<std::string> Traits;
		std::unordered_map<ENeedType, float> Needs;
		std::unordered_map<ECoreStatType, float> CoreStats;
		UTAgentContext CreateAgentContext() const;
	};
}