#include "Core/UTEffect.h"
#include "Core/UTFunctionRegistry.h"
#include <Logging/Logger.h>
#include <Math/Curves.h>

using namespace UAI;

void UTEffect::SetRawScoreFnKey(const std::string& InKey)
{
	RawScoreFn = UTFunctionRegistry::Instance().Get<ScoreFnSig>(InKey);
	RawScoreFnKey = RawScoreFn ? InKey : "";
}

void UTEffect::SetScoreCurveFnKey(const std::string& InKey)
{
	ScoreCurveFn = UTFunctionRegistry::Instance().Get<CurveFnSig>(InKey);
	ScoreCurveFnKey = ScoreCurveFn ? InKey : "";
}

void UTEffect::SetEffectFnKey(const std::string& InKey)
{
	EffectFn = UTFunctionRegistry::Instance().Get<EffectFnSig>(InKey);
	EffectFnKey = EffectFn ? InKey : "";
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
