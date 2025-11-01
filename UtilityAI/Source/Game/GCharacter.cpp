#include "GCharacter.h"
#include <Core/UTKeyGenerator.h>
#include <Core/UTObjectQuery.h>
#include <Core/UTObjectRegistry.h>
#include <Logging/UTLogger.h>
#include <UAI/UTGoal.h>
#include <UAI/UTGoalStatics.h>

using namespace Game;
using namespace UTCore;
using namespace UAI;

GCharacter::GCharacter()
{
	constexpr float MidNeed = (MIN_NEED + MAX_NEED) / 2.f;
	Needs[ENeedType::Survival] = MidNeed;
	Needs[ENeedType::Social] = MidNeed;
	Needs[ENeedType::Wealth] = MidNeed;
	Needs[ENeedType::Status] = MidNeed;
	Needs[ENeedType::Curiosity] = MidNeed;
	Needs[ENeedType::Faith] = MidNeed;

	constexpr float MidCoreStat = (MIN_CORE_STAT + MAX_CORE_STAT) / 2.f;
	CoreStats[ECoreStatType::Strength] = MidCoreStat;
	CoreStats[ECoreStatType::Agility] = MidCoreStat;
	CoreStats[ECoreStatType::Endurance] = MidCoreStat;
	CoreStats[ECoreStatType::Intellect] = MidCoreStat;
	CoreStats[ECoreStatType::Willpower] = MidCoreStat;
	CoreStats[ECoreStatType::Charisma] = MidCoreStat;

	Key = UTKeyGenerator::Instance().GenerateKey();
}

GCharacter::GCharacter(const std::string& InName)
	: GCharacter()
{
	Name = InName;
}

UTAgentContext GCharacter::CreateAgentContext() const
{
	UTAgentContext Context;
	for (const auto& [NeedType, Value] : Needs)
	{
		Context.Needs[ToString(NeedType)] = Value;
	}
	for (const auto& [StatType, Value] : CoreStats)
	{
		Context.Stats[ToString(StatType)] = Value;
	}
	return Context;
}

void GCharacter::UpdateGoals()
{
	UTObjectQuery<UAI::UTGoal> GoalQuery;
	GoalQuery.AnyTags = { "Generic", Profession };

	const auto FoundGoals = UTObjectRegistry<UAI::UTGoal>::Instance().Query(GoalQuery);
	if (FoundGoals.empty())
	{
		LOG_WARN(std::format("[GCharacter] '{}' - No available goals found!", Name))
		return;
	}

	UTAgentContext Context = CreateAgentContext();

	std::vector<UTGoalScore> TopGoals = GetTopKGoalsWithScores(FoundGoals, Context, 1);
	for (auto& GS : TopGoals)
	{
		LOG_INFO(std::format("[GCharacter] '{}' - Found Goal: '{}' - Score: {}", Name, GS.Goal->GetName(), GS.Score))
	}
}
