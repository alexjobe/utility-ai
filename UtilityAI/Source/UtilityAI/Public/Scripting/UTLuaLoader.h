#pragma once
#include "UTLuaUtils.h"
#include <Core/UTConsideration.h>
#include <Core/UTEffect.h>
#include <optional>
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

	#define LoaderArgs const sol::table& Table, const std::string& Category, UTValidationResult& Result
	using LoaderFn = std::function<void(LoaderArgs)>;
	void ActionLoader(LoaderArgs);
	void GoalLoader(LoaderArgs);

	std::optional<sol::table> LoadLuaTable(const std::filesystem::path& File, sol::state& Lua, UTValidationResult& Result);
	void LoadScriptsRecursive(const std::string& BaseDir, sol::state& Lua, UTValidationResult& Result, LoaderFn Loader);
}