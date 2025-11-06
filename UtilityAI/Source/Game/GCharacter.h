#pragma once
#include "StatTypes.h"
#include "UTAgentContext.h"
#include <string>
#include <unordered_map>
#include <UTAction.h>
#include <UTGoal.h>
#include <UTTrait.h>
#include <vector>

using namespace UAI;

namespace Game
{
	struct GCharacterRenderComponent
	{
		bool bShowInfoWindow = false;
		bool bShowTraitsWindow = false;
		bool bShowGoalsWindow = false;
		bool bShowActionsWindow = false;
	};

	class GCharacter
	{
	public:
		std::string Name = "None";
		std::string Profession = "None";
		std::unordered_map<ENeedType, float> Needs;
		std::unordered_map<ECoreStatType, float> CoreStats;

		GCharacterRenderComponent RenderComp;

		GCharacter();
		GCharacter(const std::string& InName);

		UTAgentContext CreateAgentContext() const;
		std::string GetKey() const { return Key; }

		void UpdateGoals();
		void UpdateActions();
		void AddTrait(const std::string& Trait);

		const std::vector<UTGoal>& GetCurrentGoals() const { return CurrentGoals; }
		const std::vector<UTAction>& GetCurrentActions() const { return CurrentActions; }
		const std::unordered_map<std::string, const UTTrait*>& GetCurrentTraits() const { return CurrentTraits; }

	private:
		std::string Key;
		std::vector<UTGoal> CurrentGoals;
		std::vector<UTAction> CurrentActions;
		std::unordered_map<std::string, const UTTrait*> CurrentTraits;
	};
}