#pragma once
#include "StatTypes.h"
#include "UTEffect.h"
#include <sol.hpp>
#include <UTEffectTypes.h>

using namespace UAI;

namespace Game
{
	UTEffect MakeNeedEffect(ENeedType Need, float Magnitude, float Weight = 1.0f)
	{
		UTEvaluationData Data;
		Data.Target = ToString(Need);
		Data.Raw = Magnitude;
		Data.MinRaw = MIN_NEED;
		Data.MaxRaw = MAX_NEED;
		Data.BaseWeight = Weight;
		return UAI::NeedEffect(Data);
	}

	UTConsideration MakeNeedConsideration(ENeedType Need, float Weight = 1.0f)
	{
		UTEvaluationData Data;
		Data.Target = ToString(Need);
		Data.MinRaw = MIN_NEED;
		Data.MaxRaw = MAX_NEED;
		Data.BaseWeight = Weight;
		return UAI::NeedConsideration(Data);
	}

	void RegisterLuaTypes(sol::state& Lua)
	{
		Lua.new_enum<ENeedType>("ENeedType", {
			{ ToString(ENeedType::Survival), ENeedType::Survival },
			{ ToString(ENeedType::Social), ENeedType::Social },
			{ ToString(ENeedType::Wealth), ENeedType::Wealth },
			{ ToString(ENeedType::Status), ENeedType::Status },
			{ ToString(ENeedType::Curiosity), ENeedType::Curiosity },
			{ ToString(ENeedType::Faith), ENeedType::Faith }
		});

		Lua.new_enum<ECoreStatType>("ECoreStatType", {
			{ ToString(ECoreStatType::Strength), ECoreStatType::Strength },
			{ ToString(ECoreStatType::Agility), ECoreStatType::Agility },
			{ ToString(ECoreStatType::Endurance), ECoreStatType::Endurance },
			{ ToString(ECoreStatType::Intellect), ECoreStatType::Intellect },
			{ ToString(ECoreStatType::Willpower), ECoreStatType::Willpower },
			{ ToString(ECoreStatType::Charisma), ECoreStatType::Charisma },
			});

		Lua.set_function("_MakeNeedEffect", &MakeNeedEffect);
		Lua.set_function("_MakeNeedConsideration", &MakeNeedConsideration);
	}
}