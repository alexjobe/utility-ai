#pragma once
#include "StatTypes.h"
#include <UtilityAI.h>
#include <memory>

namespace Game
{
	std::unique_ptr<UTEffect> MakeNeedEffect(ENeedType Need, float Magnitude)
	{
		auto NewEffect = std::make_unique<UTEffect_Need>();
		NewEffect->Name = "Effect_" + ToString(Need);
		NewEffect->Data.Target = ToString(Need);
		NewEffect->Data.Magnitude = Magnitude;
		NewEffect->Data.MinRaw = MIN_NEED;
		NewEffect->Data.MaxRaw = MAX_NEED;
		return NewEffect;
	}
}