#pragma once
#include "Core/UTAgentContext.h"
#include "StatTypes.h"
#include <unordered_map>

using namespace UAI;

namespace Game
{
	class Character
	{
	public:
		Character();
		std::unordered_map<ENeedType, float> Needs;
		std::unordered_map<ECoreStatType, float> CoreStats;
		UTAgentContext CreateUtilityContext() const;
	};
}