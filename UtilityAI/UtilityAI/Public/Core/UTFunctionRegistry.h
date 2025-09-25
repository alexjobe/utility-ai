#pragma once
#include "UTConsideration.h"
#include <string>

namespace UAI
{
class UTFunctionRegistry
{
public:
    static UTFunctionRegistry& Instance();

    void RegisterScoreFn(const std::string& Name, ScoreFn Fn);
    void RegisterEffectFn(const std::string& Name, std::function<void(UTAgentContext&, const UTEvaluationData&)> Fn);

    ScoreFn GetScoreFn(const std::string& Name) const;
    std::function<void(UTAgentContext&, const UTEvaluationData&)> GetEffectFn(const std::string& Name) const;

private:
    std::unordered_map<std::string, ScoreFn> ScoreFns;
    std::unordered_map<std::string, std::function<void(UTAgentContext&, const UTEvaluationData&)>> EffectFns;

	UTFunctionRegistry() = default;
	~UTFunctionRegistry() = default;
	UTFunctionRegistry(const UTFunctionRegistry&) = delete;
	UTFunctionRegistry& operator=(const UTFunctionRegistry&) = delete;
	UTFunctionRegistry(UTFunctionRegistry&&) = delete;
	UTFunctionRegistry& operator=(UTFunctionRegistry&&) = delete;
};

}