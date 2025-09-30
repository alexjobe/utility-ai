#pragma once
#include "Math/Curves.h"
#include "UTAgentContext.h"
#include "UTConsideration.h"
#include <functional>
#include <string>

namespace UAI
{
using EffectFnSig = void(UTAgentContext&, const UTEvaluationData&);
using EffectFnType = std::function<EffectFnSig>;

class UTEffect
{
public:
	std::string Key;
	std::string ConsiderationKey;
	UTEvaluationData Data;
	bool bIsConsideration = false;

	void SetRawScoreFnKey(const std::string& InKey);
	void SetScoreCurveFnKey(const std::string& InKey);
	void SetEffectFnKey(const std::string& InKey);

	void Apply(UTAgentContext& InContext) const;

	virtual UTConsideration AsConsideration() const;

private:
	std::string RawScoreFnKey;
	std::string ScoreCurveFnKey;
	std::string EffectFnKey;
	const ScoreFnType* RawScoreFn = nullptr;
	const CurveFnType* ScoreCurveFn = nullptr;
	const EffectFnType* EffectFn = nullptr;
};
}