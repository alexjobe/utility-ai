#pragma once
#include "UTConsideration.h"
#include "UTEffect.h"
#include "UTFunctionRegistry.h"
#include <algorithm>

namespace UAI
{
inline float ScoreNeedChange(const UTAgentContext& Context, const UTEvaluationData& Data)
{
	const float Before = Context.GetNeed(Data.Target);

	const float After = std::clamp(Before - Data.Raw, Data.MinRaw, Data.MaxRaw);
	const float PercentChange = (After - Before) / (Data.MaxRaw - Data.MinRaw);

	const float BeforeNorm = UTMath::Normalize(Before, Data.MinRaw, Data.MaxRaw);
	const float RawScore = BeforeNorm * -PercentChange; // Favor smaller values

	return std::clamp(RawScore, 0.0f, 1.0f);
}

inline void ApplyNeedChange(UTAgentContext& Context, const UTEvaluationData& Data)
{
	Context.Needs[Data.Target] -= Data.Raw;
	Context.Needs[Data.Target] = std::clamp(Context.Needs[Data.Target], Data.MinRaw, Data.MaxRaw);
}

// Convenience function for need effects
inline UTEffect NeedEffect(const std::string& Need, float Magnitude, float MinNeed = 0.f, float MaxNeed = 1.f)
{
	UTEffect NewEffect;
	NewEffect.Key = "Effect." + Need;
	NewEffect.ConsiderationKey = "Need." + Need;
	NewEffect.bIsConsideration = true;
	NewEffect.SetRawScoreFnKey("ScoreNeedChange");
	NewEffect.SetEffectFnKey("ApplyNeedChange");
	NewEffect.Data.Target = Need;
	NewEffect.Data.Raw = Magnitude;
	NewEffect.Data.MinRaw = MinNeed;
	NewEffect.Data.MaxRaw = MaxNeed;
	return NewEffect;
}

inline void RegisterEffectTypes()
{
	UTFunctionRegistry::Instance().Register<ScoreFnSig>(
		"ScoreNeedChange",
		&ScoreNeedChange
	);
	UTFunctionRegistry::Instance().Register<EffectFnSig>(
		"ApplyNeedChange",
		&ApplyNeedChange
	);
}
}