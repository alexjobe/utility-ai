#pragma once
#include "UTLuaUtils.h"
#include <optional>
#include <sol.hpp>
#include <string>
#include <UTConsideration.h>
#include <UTEffect.h>

using namespace UAI;
using namespace UTLuaUtils;

namespace UTLuaLoader
{
	void RegisterScoreFunction(const std::string& Key, sol::function Fn);
	void RegisterEffectFunction(const std::string& Key, sol::function Fn);
	void RegisterCurveFunction(const std::string& Key, sol::function Fn);
	void RegisterLuaTypes(sol::state& Lua);

	UTEvaluationData LoadEvaluationData(const sol::table& Table, UTScriptResult& Result);
	UTConsideration LoadConsideration(const sol::table& Table, UTScriptResult& Result);
	UTEffect LoadEffect(const sol::table& Table, UTScriptResult& Result);
	UTBias LoadBias(const sol::table& Table, UTScriptResult& Result);

	#define LoaderArgs const sol::table& Table, UTScriptResult& Result
	using LoaderFn = std::function<void(LoaderArgs)>;
	void ActionLoader(LoaderArgs);
	void GoalLoader(LoaderArgs);
	void TraitLoader(LoaderArgs);

	void LoadAllScripts(sol::state& Lua);
	bool LoadScriptsRecursive(const std::string& BaseDir, sol::state& Lua, std::vector<UTScriptResult>& Results, LoaderFn Loader);
	std::optional<sol::table> LoadLuaTable(sol::state& Lua, UTScriptResult& Result);

	template <typename AddFn>
	void LoadConsiderations(const sol::table& Table, UTScriptResult& Result, AddFn Add)
	{
		if (const auto Considerations = ValidateField<sol::table>(Table, "Considerations", Result))
		{
			for (auto& [_, Cons] : *Considerations)
			{
				if (Cons.get_type() == sol::type::table)
				{
					Add(LoadConsideration(Cons, Result));
				}
				else if (Cons.is<UTConsideration>())
				{
					Add(Cons.as<UTConsideration>());
				}
				else
				{
					Result.AddError("Unexpected consideration type in Considerations array");
				}
			}
		}
	}

	template <typename AddFn>
	void LoadEffects(const sol::table& Table, UTScriptResult& Result, AddFn Add)
	{
		if (const auto Effects = ValidateField<sol::table>(Table, "Effects", Result))
		{
			for (auto& [_, Effect] : *Effects)
			{
				if (Effect.get_type() == sol::type::table)
				{
					Add(LoadEffect(Effect, Result));
				}
				else if (Effect.is<UTEffect>())
				{
					Add(Effect.as<UTEffect>());
				}
				else
				{
					Result.AddError("Unexpected effect type in Effects array");
				}
			}
		}
	}

	template <typename AddFn>
	void LoadBiases(const sol::table& Table, UTScriptResult& Result, AddFn Add)
	{
		if (const auto Biases = ValidateField<sol::table>(Table, "Biases", Result))
		{
			for (auto& [_, Bias] : *Biases)
			{
				if (Bias.get_type() == sol::type::table)
				{
					Add(LoadBias(Bias, Result));
				}
				else if (Bias.is<UTBias>())
				{
					Add(Bias.as<UTBias>());
				}
				else
				{
					Result.AddError("Unexpected bias type in Biases array");
				}
			}
		}
	}

	template <typename AddFn>
	void LoadTags(const sol::table& Table, const std::string& Field, UTScriptResult& Result, AddFn Add)
	{
		if (const auto Tags = ValidateField<sol::table>(Table, Field, Result))
		{
			for (auto& [_, Tag] : *Tags)
			{
				if (Tag.get_type() == sol::type::string)
				{
					Add(Tag.as<std::string>());
				}
				else
				{
					Result.AddError("Invalid tag type (expected string)");
				}
			}
		}
	}
}