#include "Core/UTAction.h"
#include "Core/UTScorer.h"
#include "Game/Character.h"
#include "Game/GameHelpers.h"
#include "Game/StatTypes.h"
#include "Logging/Logger.h"
#include "UtilityAI.h"
#include <memory>
#include <string>

using namespace UtilityAI;
using namespace Game;
using namespace Log;

int main()
{
	Logger::Instance().AddSink(std::make_shared<ConsoleSink>(EVerbosity::Minimal));

	Character MyCharacter;

	UTAction RaidAction("Action.Raid");
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
	RaidAction.Scorer.AddConsideration(SuccessChanceCons);

	UTConsideration TimeCostCons;
	TimeCostCons.Key = "TimeCost";
	TimeCostCons.Data.Raw = 4; // Example: 4 turns, out of a max 5 
	TimeCostCons.Data.MinRaw = 1;
	TimeCostCons.Data.MaxRaw = 5;
	TimeCostCons.ScoreCurve = [](float x) { return 1.f - x; }; // Lower time = better
	TimeCostCons.Data.Weight = 0.5f;
	RaidAction.Scorer.AddConsideration(TimeCostCons);

	// Generates need considerations from effects
	RaidAction.GenerateConsiderations();

	UTAgentContext MyContext = MyCharacter.CreateUtilityContext();

	RaidAction.Scorer.Score(MyContext);

	RaidAction.Execute(MyContext);
}