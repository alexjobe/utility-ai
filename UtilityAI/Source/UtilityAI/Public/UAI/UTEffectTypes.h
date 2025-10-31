#pragma once
#include "UTConsideration.h"
#include "UTEffect.h"
#include "UTFunctionRegistry.h"
#include <algorithm>
#include <Math/UTMath.h>

namespace UAI
{
inline float ScoreRawNeed(const UTAgentContext& Context, const UTEvaluationData& Data)
{
	const float Raw = Context.GetNeed(Data.Target);
	return UTMath::Normalize(Raw, Data.MinRaw, Data.MaxRaw);
}

inline float ScoreNeedChange(const UTAgentContext& Context, const UTEvaluationData& Data)
{
	const float Before = Context.GetNeed(Data.Target);

	const float After = std::clamp(Before - Data.Raw, Data.MinRaw, Data.MaxRaw);
	const float PercentChange = (After - Before) / (Data.MaxRaw - Data.MinRaw);

	const float BeforeNorm = UTMath::Normalize(Before, Data.MinRaw, Data.MaxRaw);
	const float RawScore = BeforeNorm * -PercentChange; // Favor smaller need values

	return std::clamp(RawScore, 0.0f, 1.0f);
}

inline void ApplyNeedChange(UTAgentContext& Context, const UTEvaluationData& Data)
{
	Context.Needs[Data.Target] -= Data.Raw;
	Context.Needs[Data.Target] = std::clamp(Context.Needs[Data.Target], Data.MinRaw, Data.MaxRaw);
}

// Convenience function for need effects
inline UTEffect NeedEffect(const UTEvaluationData& Data)
{
	UTEffect NewEffect;
	NewEffect.Key = "Effect." + Data.Target;
	NewEffect.ConsiderationKey = "Need." + Data.Target;
	NewEffect.bIsConsideration = true;
	NewEffect.SetRawScoreFnKey("ScoreNeedChange");
	NewEffect.SetEffectFnKey("ApplyNeedChange");
	NewEffect.Data = Data;
	return NewEffect;
}

// Convenience function for need considerations
inline UTConsideration NeedConsideration(const UTEvaluationData& Data)
{
	UTConsideration NewConsideration;
	NewConsideration.Key = "Need." + Data.Target;
	NewConsideration.Data = Data;
	NewConsideration.SetRawScoreFnKey("ScoreRawNeed");
	return NewConsideration;
}

inline void RegisterEffectTypes()
{
	UTFunctionRegistry::Instance().Register<ScoreFnSig>(
		"ScoreRawNeed",
		&ScoreRawNeed
	);

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