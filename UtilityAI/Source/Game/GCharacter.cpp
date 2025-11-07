#include "GCharacter.h"
#include <Core/UTKeyGenerator.h>
#include <Core/UTObjectQuery.h>
#include <Core/UTObjectRegistry.h>
#include <Logging/UTLogger.h>
#include <utility>
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

	UTObjectQuery<UTGoal> GoalQuery;
	GoalQuery.AnyTags = { "Generic", Profession };

	const std::vector<const UTGoal*> FoundGoals = UTObjectRegistry<UTGoal>::Instance().Query(GoalQuery);
	if (FoundGoals.empty())
	{
		LOG_WARN(std::format("[GCharacter] '{}' - No available goals found!", Name))
		return;
	}

	std::vector<UTGoal> TraitModifiedGoals;
	for (const auto* Goal : FoundGoals)
	{
		UTGoal ModifiedGoal(*Goal);
		for (const auto& [_, Trait] : CurrentTraits)
		{
			Trait->ApplyToGoal(ModifiedGoal);
		}
		TraitModifiedGoals.push_back(std::move(ModifiedGoal));
	}

	UTAgentContext Context = CreateAgentContext();

	CurrentGoals = GetTopKWithScores(std::move(TraitModifiedGoals), Context, 1);
	for (const auto& Goal : CurrentGoals)
	{
		LOG_INFO(std::format("[GCharacter] '{}' - Found Goal: '{}' - Score: {}", Name, Goal.Object->GetName(), Goal.Score))
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
		for (const auto& Tag : Goal.Object->RequiredTags)
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

	std::vector<UTAction> TraitModifiedActions;
	for (const auto* Action : FoundActions)
	{
		UTAction ModifiedAction(*Action);
		for (const auto& [_, Trait] : CurrentTraits)
		{
			Trait->ApplyToAction(ModifiedAction);
		}
		TraitModifiedActions.push_back(std::move(ModifiedAction));
	}

	UTAgentContext Context = CreateAgentContext();

	CurrentActions = GetTopKWithScores(std::move(TraitModifiedActions), Context, 1);
	for (const auto& Action : CurrentActions)
	{
		LOG_INFO(std::format("[GCharacter] '{}' - Found Action: '{}' - Score: {}", Name, Action.Object->GetName(), Action.Score))
	}
}

void GCharacter::AddTrait(const std::string& Trait)
{
	if (auto It = CurrentTraits.find(Trait); It == CurrentTraits.end())
	{
		const auto FoundTraits = UTObjectRegistry<UTTrait>::Instance().FindAllWithName(Trait);
		if (!FoundTraits.empty())
		{
			CurrentTraits[Trait] = FoundTraits[0];
		}
		if (FoundTraits.size() > 1)
		{
			LOG_WARN(std::format("[GCharacter] Multiple traits found with name '{}'", Trait))
		}
	}
	else
	{
		LOG_WARN(std::format("[GCharacter] Attempted to add duplicate trait '{}'", Trait))
	}
}
