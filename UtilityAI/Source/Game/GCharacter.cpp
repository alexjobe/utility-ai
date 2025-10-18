#include "GCharacter.h"
#include <Core/UTKeyGenerator.h>

using namespace Game;
using namespace UTCore;

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