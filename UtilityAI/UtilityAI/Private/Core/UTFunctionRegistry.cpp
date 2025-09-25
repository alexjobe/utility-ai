#include <Core/UTFunctionRegistry.h>

using namespace UAI;

UTFunctionRegistry& UTFunctionRegistry::Instance()
{
	static UTFunctionRegistry Singleton;
	return Singleton;
}

void UTFunctionRegistry::RegisterScoreFn(const std::string& Name, ScoreFn Fn)
{
	ScoreFns[Name] = Fn;
}

void UTFunctionRegistry::RegisterEffectFn(const std::string& Name, std::function<void(UTAgentContext&, const UTEvaluationData&)> Fn)
{
	EffectFns[Name] = Fn;
}

ScoreFn UTFunctionRegistry::GetScoreFn(const std::string& Name) const
{
	auto It = ScoreFns.find(Name);
	return It != ScoreFns.end() ? It->second : nullptr;
}

std::function<void(UAI::UTAgentContext&, const UAI::UTEvaluationData&)> UTFunctionRegistry::GetEffectFn(const std::string& Name) const
{
	auto It = EffectFns.find(Name);
	return It != EffectFns.end() ? It->second : nullptr;
}

