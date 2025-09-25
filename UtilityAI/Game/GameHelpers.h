#pragma once
#include "Core/UTEffect.h"
#include "StatTypes.h"
#include <sol.hpp>

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

		Lua.set_function("_MakeNeedEffect", &MakeNeedEffect);
	}
}