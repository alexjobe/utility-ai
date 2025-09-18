#pragma once
#include "UTLuaUtils.h"
#include <Core/UTAction.h>
#include <Core/UTActionRegistry.h>
#include <Core/UTAgentContext.h>
#include <filesystem>
#include <Logging/Logger.h>
#include <optional>
#include <sol.hpp>
#include <string>

using namespace LuaUtils;

namespace UAI
{
	void BindAgentContext(sol::state& Lua) 
	{
		Lua.new_usertype<UTAgentContext>(
			"UTAgentContext",
			"GetNeed", &UTAgentContext::GetNeed,
			"SetNeed", &UTAgentContext::SetNeed,
			"GetStat", &UTAgentContext::GetStat,
			"SetStat", &UTAgentContext::SetStat
		);
	}

	UTEvaluationData LoadEvaluationData(const sol::table& Table, UTValidationResult& Result)
	{
		UTEvaluationData Data;
		LOAD_FIELD(Data, Target, Table, Result, false);
		LOAD_FIELD(Data, Raw, Table, Result, false);
		LOAD_FIELD(Data, MinRaw, Table, Result, false);
		LOAD_FIELD(Data, MaxRaw, Table, Result, false);
		LOAD_FIELD(Data, Weight, Table, Result, false);
		LOAD_FIELD(Data, Priority, Table, Result, false);
		return Data;
	}

	UTEffect LoadEffect(const sol::table& Table, UTValidationResult& Result)
	{
		UTEffect Effect;
		LOAD_FIELD(Effect, Key, Table, Result, true);
		LOAD_FIELD(Effect, ConsiderationKey, Table, Result, false);
		LOAD_FIELD(Effect, EvalRawScoreFn, Table, Result, false);
		LOAD_FIELD(Effect, bIsConsideration, Table, Result, false);
		LOAD_FIELD(Effect, ScoreCurve, Table, Result, false);
		LOAD_FIELD(Effect, EffectFn, Table, Result, false);
		return Effect;
	}

	UTAction LoadAction(const sol::table& Table, UTValidationResult& Result)
	{
		UTAction Action;
		if (const auto Key = ValidateField<std::string>(Table, "Key", Result, true))
		{
			Action.SetKey(*Key);
		}

		if (const auto Tags = ValidateField<sol::table>(Table, "Tags", Result))
		{
			for (auto& [_, Tag] : *Tags)
			{
				if (Tag.get_type() == sol::type::string)
				{
					Action.Tags.insert(Tag.as<std::string>());
				}
				else
				{
					Result.AddError("Invalid tag type (expected string)");
				}
			}
		}

		if (const auto Effects = ValidateField<sol::table>(Table, "Effects", Result))
		{
			for (auto& [_, Effect] : *Effects)
			{
				if (Effect.get_type() == sol::type::table)
				{
					Action.AddEffect(LoadEffect(Effect, Result));
				}
			}
		}

		Action.GenerateConsiderations();
		return Action;
	}

	void LoadActionsRecursive(const std::string& BaseDir, sol::state& Lua) 
	{
		try 
		{
			for (auto& Entry : std::filesystem::recursive_directory_iterator(BaseDir)) 
			{
				if (!Entry.is_regular_file() || Entry.path().extension() != ".lua") continue;

				// Category = relative folder from BaseDir
				std::filesystem::path Relative = std::filesystem::relative(Entry.path().parent_path(), BaseDir);
				std::string Category = Relative.empty() ? "Uncategorized" : Relative.string();

				try 
				{
					sol::load_result Script = Lua.load_file(Entry.path().string());
					if (!Script.valid()) continue;

					sol::protected_function_result Result = Script();
					if (!Result.valid()) continue;

					sol::table ActionTable = Result;
					UTValidationResult ValidationResult;
					UTAction Action = LoadAction(ActionTable, ValidationResult);
					UTActionRegistry::Instance().Register(Action, Category);

					LOG_INFO(std::format("Loaded action: {} (Category: {})", Action.GetKey(), Category))
				}
				catch (const std::exception& Error) 
				{
					LOG_ERROR(std::format("Failed to load action: {} Error: {}", Entry.path().string(), Error.what()))
				}
			}
		}
		catch (const std::exception& Error)
		{
			LOG_ERROR(std::format("Filesystem Error: {}", Error.what()))
		}
	}

}