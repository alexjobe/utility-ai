#include "Character.h"
#include "Math/Math.h"

using namespace Game;

Character::Character()
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
}

UtCharacterContext Character::CreateUtilityContext() const
{
	UtCharacterContext Context;
	for (const auto& [NeedType, Value] : Needs)
	{
		Context.Needs[ToString(NeedType)] = Normalize(Value, MIN_NEED, MAX_NEED);
	}
	for (const auto& [StatType, Value] : CoreStats)
	{
		Context.Needs[ToString(StatType)] = Normalize(Value, MIN_CORE_STAT, MAX_CORE_STAT);
	}
	return Context;
}