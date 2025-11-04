#pragma once
#include "StatTypes.h"
#include "UTAgentContext.h"
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <UTAction.h>
#include <UTGoal.h>
#include <vector>

using namespace UAI;

namespace Game
{
	struct GCharacterRenderComponent
	{
		bool bShowInfoWindow = false;
		bool bShowGoalsWindow = false;
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
		void UpdateActions();

		const std::vector<UTGoal>& GetCurrentGoals() const { return CurrentGoals; }
		const std::vector<UTAction>& GetCurrentActions() const { return CurrentActions; }

	private:
		std::string Key;
		std::vector<UTGoal> CurrentGoals;
		std::vector<UTAction> CurrentActions;
	};
}