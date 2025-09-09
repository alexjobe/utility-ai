#include "Game/Character.h"
#include "Game/GameHelpers.h"
#include "Game/StatTypes.h"
#include "UtilityAI.h"
#include <Core/UTAction.h>
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
	RaidAction.AddEffect(MakeNeedEffect(ENeedType::Wealth, 30.f));
	RaidAction.AddEffect(MakeNeedEffect(ENeedType::Survival, -10.f));

	UTConsideration SuccessChanceCons;
	SuccessChanceCons.Key = "SuccessChance";
	SuccessChanceCons.EvalRawScore = [](const UTAgentContext& Ctx, const UTEvaluationData&)
		{
			const float Strength = Ctx.GetStat(ToString(ECoreStatType::Strength));
			const float Endurance = Ctx.GetStat(ToString(ECoreStatType::Endurance));
			return (0.6f * Strength + 0.4f * Endurance);
		};

	SuccessChanceCons.Data.Weight = 1.5f;
	RaidAction.AddConsideration(SuccessChanceCons);

	UTConsideration TimeCostCons;
	TimeCostCons.Key = "TimeCost";
	TimeCostCons.Data.Magnitude = 4; // Example: 4 turns, out of a max 5 
	TimeCostCons.Data.MinRaw = 1;
	TimeCostCons.Data.MaxRaw = 5;
	TimeCostCons.ScoreCurve = [](float x) { return 1.f - x; }; // Lower time = better
	TimeCostCons.Data.Weight = 0.5f;
	RaidAction.AddConsideration(TimeCostCons);

	// Generates need considerations from effects
	RaidAction.GenerateConsiderations();
}