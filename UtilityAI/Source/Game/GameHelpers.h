#pragma once
#include "StatTypes.h"
#include "UAI/UTEffect.h"
#include <sol.hpp>
#include <UAI/UTEffectTypes.h>

using namespace UAI;

namespace Game
{
	UTEffect MakeNeedEffect(ENeedType Need, float Magnitude)
	{
		return UAI::NeedEffect(ToString(Need), Magnitude, MIN_NEED, MAX_NEED);
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
	}
}