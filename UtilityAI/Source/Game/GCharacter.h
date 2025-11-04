#pragma once
#include "StatTypes.h"
#include "UAI/UTAgentContext.h"
#include <string>
#include <UAI/UTGoal.h>
#include <unordered_map>
#include <unordered_set>
#include <vector>

using namespace UAI;

namespace Game
{
	struct GCharacterRenderComponent
	{
		bool bShowGoals = false;
	};

	class GCharacter
	{
	public:
		std::string Name = "None";
		std::string Profession = "None";
		std::unordered_set<std::string> Traits;
		std::unordered_map<ENeedType, float> Needs;
		std::unordered_map<ECoreStatType, float> CoreStats;

		GCharacterRenderComponent RenderComp;

		GCharacter();
		GCharacter(const std::string& InName);

		UTAgentContext CreateAgentContext() const;
		std::string GetKey() const { return Key; }

		void UpdateGoals();

		const std::vector<UTGoal>& GetCurrentGoals() const { return CurrentGoals; }

	private:
		std::string Key;
		std::vector<UTGoal> CurrentGoals;
	};
}