#pragma once
#include "Core/UTEffect.h"
#include "StatTypes.h"

namespace Game
{
	UTEffect MakeNeedEffect(ENeedType Need, float Magnitude)
	{
		return UtilityAI::NeedEffect(ToString(Need), Magnitude, MIN_NEED, MAX_NEED);
	}
}