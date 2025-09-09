#pragma once
#include "StatTypes.h"
#include <UtilityAI.h>

namespace Game
{
	UTEffect_Need MakeNeedEffect(ENeedType Need, float Magnitude)
	{
		UTEffect_Need NewEffect;
		NewEffect.Params.Target = ToString(Need);
		NewEffect.Params.Magnitude = Magnitude;
		NewEffect.Params.MinRaw = MIN_NEED;
		NewEffect.Params.MaxRaw = MAX_NEED;
		return NewEffect;
	}
}