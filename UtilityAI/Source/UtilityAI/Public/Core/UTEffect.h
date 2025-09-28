#pragma once
#include "Logging/Logger.h"
#include "Math/Curves.h"
#include "Math/Math.h"
#include "UTAgentContext.h"
#include "UTConsideration.h"
#include <algorithm>
#include <format>
#include <functional>
#include <string>

namespace UAI
{
class UTEffect
{
public:
	std::string Key;
	std::string ConsiderationKey;
	UTEvaluationData Data;
	ScoreFn EvalRawScoreFn = nullptr;
	CurveFn ScoreCurveFn = nullptr;

	bool bIsConsideration = false;

	std::function<void(UTAgentContext&, const UTEvaluationData&)> EffectFn = nullptr;

	void Apply(UTAgentContext& Context) const
	{
		if (EffectFn)
		{
			EffectFn(Context, Data);
		}
		else
		{
			LOG_WARN(std::format("Effect: {} - EffectFn not set!", Key))
		}
	}

	virtual UTConsideration AsConsideration() const
	{
		return { ConsiderationKey, Data, EvalRawScoreFn, ScoreCurveFn };
	}
};

inline float ScoreNeedChange(const UTAgentContext& Context, const UTEvaluationData& Data)
{
	const float Before = Context.GetNeed(Data.Target);

	const float After = std::clamp(Before - Data.Raw, Data.MinRaw, Data.MaxRaw);
	const float PercentChange = (After - Before) / (Data.MaxRaw - Data.MinRaw);

	const float BeforeNorm = Normalize(Before, Data.MinRaw, Data.MaxRaw);
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
	NewEffect.EvalRawScoreFn = UAI::ScoreNeedChange;
	NewEffect.bIsConsideration = true;
	NewEffect.EffectFn = UAI::ApplyNeedChange;
	NewEffect.Data.Target = Need;
	NewEffect.Data.Raw = Magnitude;
	NewEffect.Data.MinRaw = MinNeed;
	NewEffect.Data.MaxRaw = MaxNeed;
	return NewEffect;
}
}