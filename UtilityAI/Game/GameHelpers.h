#pragma once
#include "StatTypes.h"
#include <UtilityAI.h>

namespace Game
{
	UTEffect MakeNeedEffect(ENeedType Need, float Magnitude)
	{
		UTEffect NewEffect;
		NewEffect.Name = "Effect_" + ToString(Need);
		NewEffect.ConsiderationKey = "Need_" + ToString(Need);
		NewEffect.EvalRawScore = UtilityAI::ScoreNeedChange;
		NewEffect.EffectFn = UtilityAI::ApplyNeedChange;
		NewEffect.Data.Target = ToString(Need);
		NewEffect.Data.Magnitude = Magnitude;
		NewEffect.Data.MinRaw = MIN_NEED;
		NewEffect.Data.MaxRaw = MAX_NEED;
		return NewEffect;
	}
}