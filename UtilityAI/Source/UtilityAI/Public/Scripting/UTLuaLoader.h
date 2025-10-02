#pragma once
#include "UTLuaUtils.h"
#include <Core/UTAction.h>
#include <Core/UTConsideration.h>
#include <Core/UTEffect.h>
#include <sol.hpp>
#include <string>

using namespace UAI;
using namespace LuaUtils;

namespace UTLoader
{
	void RegisterScoreFunction(const std::string& Key, sol::function Fn);

	void RegisterEffectFunction(const std::string& Key, sol::function Fn);

	void RegisterCurveFunction(const std::string& Key, sol::function Fn);

	void RegisterLuaTypes(sol::state& Lua);

	UTEvaluationData LoadEvaluationData(const sol::table& Table, UTValidationResult& Result);

	UTConsideration LoadConsideration(const sol::table& Table, UTValidationResult& Result);

	UTEffect LoadEffect(const sol::table& Table, UTValidationResult& Result);

	UTAction LoadAction(const sol::table& Table, UTValidationResult& Result);

	void LoadActionsRecursive(const std::string& BaseDir, sol::state& Lua, UTValidationResult& Result);
}