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
	void TraitLoader(LoaderArgs);

	std::optional<sol::table> LoadLuaTable(const std::filesystem::path& File, sol::state& Lua, UTValidationResult& Result);
	void LoadScriptsRecursive(const std::string& BaseDir, sol::state& Lua, UTValidationResult& Result, LoaderFn Loader);

	template <typename AddFn>
	void LoadConsiderations(const sol::table& Table, UTValidationResult& Result, AddFn Add)
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
	void LoadEffects(const sol::table& Table, UTValidationResult& Result, AddFn Add)
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
	void LoadTags(const sol::table& Table, UTValidationResult& Result, AddFn Add)
	{
		if (const auto Tags = ValidateField<sol::table>(Table, "Tags", Result))
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