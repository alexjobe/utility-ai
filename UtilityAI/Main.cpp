#include "Game/Character.h"
#include "Game/GameHelpers.h"
#include "Game/StatTypes.h"
#include "UtilityAI.h"
#include <iostream>
#include <string>
#include <vector>

using namespace UtilityAI;
using namespace Game;

int main()
{
	Character MyCharacter;

	UTAction RaidAction;
	RaidAction.Name = "Raid";
	RaidAction.Effects.push_back(MakeNeedEffect(ENeedType::Wealth, 30.f));
	RaidAction.Effects.push_back(MakeNeedEffect(ENeedType::Survival, -10.f));

	UTConsideration SuccessChance;
	SuccessChance.Key = "SuccessChance";
	SuccessChance.EvalRawScore = [](const UTAgentContext& Ctx, const UTEvaluationParams&)
		{
			const float Strength = Ctx.GetStat(ToString(ECoreStatType::Strength));
			const float Endurance = Ctx.GetStat(ToString(ECoreStatType::Endurance));
			return (0.6f * Strength + 0.4f * Endurance);
		};

	SuccessChance.Params.Weight = 1.5f;

	RaidAction.AddConsideration(SuccessChance);

}