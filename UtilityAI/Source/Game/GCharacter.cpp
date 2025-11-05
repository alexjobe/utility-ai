#include "GCharacter.h"
#include <Core/UTKeyGenerator.h>
#include <Core/UTObjectQuery.h>
#include <Core/UTObjectRegistry.h>
#include <Logging/UTLogger.h>
#include <UTObjectScore.h>

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
	CurrentGoals.clear();

	UTObjectQuery<UAI::UTGoal> GoalQuery;
	GoalQuery.AnyTags = { "Generic", Profession };

	const auto FoundGoals = UTObjectRegistry<UAI::UTGoal>::Instance().Query(GoalQuery);
	if (FoundGoals.empty())
	{
		LOG_WARN(std::format("[GCharacter] '{}' - No available goals found!", Name))
		return;
	}

	UTAgentContext Context = CreateAgentContext();

	std::vector<UTObjectScore<UTGoal>> TopScores = GetTopKWithScores(FoundGoals, Context, 1);
	for (const auto& Score : TopScores)
	{
		CurrentGoals.push_back(*Score.Object);
		LOG_INFO(std::format("[GCharacter] '{}' - Found Goal: '{}' - Score: {}", Name, Score.Object->GetName(), Score.Score))
	}
}

void GCharacter::UpdateActions()
{
	CurrentActions.clear();

	UTObjectQuery<UTAction> ActionQuery;
	ActionQuery.AnyTags = { "Generic", Profession };

	if (CurrentGoals.empty())
	{
		LOG_WARN(std::format("[GCharacter] '{}' - Cannot update actions without goals!", Name))
		return;
	}

	for (const auto& Goal : CurrentGoals)
	{
		for (const auto& Tag : Goal.RequiredTags)
		{
			ActionQuery.RequiredTags.insert(Tag);
		}
	}

	const auto FoundActions = UTObjectRegistry<UTAction>::Instance().Query(ActionQuery);
	if (FoundActions.empty())
	{
		LOG_WARN(std::format("[GCharacter] '{}' - No available actions found!", Name))
		return;
	}

	UTAgentContext Context = CreateAgentContext();

	std::vector<UTObjectScore<UTAction>> TopScores = GetTopKWithScores(FoundActions, Context, 1);
	for (const auto& Score : TopScores)
	{
		CurrentActions.push_back(*Score.Object);
		LOG_INFO(std::format("[GCharacter] '{}' - Found Action: '{}' - Score: {}", Name, Score.Object->GetName(), Score.Score))
	}
}
