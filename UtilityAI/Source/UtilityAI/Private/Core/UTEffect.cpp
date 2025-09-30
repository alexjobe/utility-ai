#include "Core/UTEffect.h"
#include "Core/UTFunctionRegistry.h"
#include <Logging/Logger.h>
#include <Math/Curves.h>

using namespace UAI;

void UTEffect::SetRawScoreFnKey(const std::string& InKey)
{
	RawScoreFnKey = InKey;
	if (!RawScoreFnKey.empty())
	{
		RawScoreFn = UTFunctionRegistry::Instance().Get<ScoreFnSig>(InKey);
	}
	else
	{
		RawScoreFn = nullptr;
	}
}

void UTEffect::SetScoreCurveFnKey(const std::string& InKey)
{
	ScoreCurveFnKey = InKey;
	if (!ScoreCurveFnKey.empty())
	{
		ScoreCurveFn = UTFunctionRegistry::Instance().Get<CurveFnSig>(InKey);
	}
	else
	{
		ScoreCurveFn = nullptr;
	}
}

void UTEffect::SetEffectFnKey(const std::string& InKey)
{
	EffectFnKey = InKey;
	if (!EffectFnKey.empty())
	{
		EffectFn = UTFunctionRegistry::Instance().Get<EffectFnSig>(InKey);
	}
	else
	{
		EffectFn = nullptr;
	}
}

void UTEffect::Apply(UTAgentContext& InContext) const
{
	if (EffectFn && *EffectFn)
	{
		(*EffectFn)(InContext, Data);
	}
	else
	{
		LOG_WARN(std::format("Effect: '{}' - EffectFn not set!", Key))
	}
}

UTConsideration UTEffect::AsConsideration() const
{
	UTConsideration Consideration;
	Consideration.Key = ConsiderationKey;
	Consideration.Data = Data;
	Consideration.SetRawScoreFnKey(RawScoreFnKey);
	Consideration.SetScoreCurveFnKey(ScoreCurveFnKey);
	return Consideration;
}
