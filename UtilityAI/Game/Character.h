#pragma once
#include "StatTypes.h"
#include "UtilityAI.h"
#include <unordered_map>

using namespace UtilityAI;

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