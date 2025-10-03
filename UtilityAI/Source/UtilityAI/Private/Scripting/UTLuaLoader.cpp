#include "Core/UTFunctionRegistry.h"
#include "Scripting/UTLuaLoader.h"
#include <Core/UTAction.h>
#include <Core/UTActionRegistry.h>
#include <filesystem>

using namespace UAI;
using namespace LuaUtils;

void UTLoader::RegisterScoreFunction(const std::string& Key, sol::function Fn)
{
	UTFunctionRegistry::Instance().Register<ScoreFnSig>(
		Key,
		WrapLuaFunction<ScoreFnSig>(std::move(Fn), Key)
	);
}

void UTLoader::RegisterEffectFunction(const std::string& Key, sol::function Fn)
{
	UTFunctionRegistry::Instance().Register<EffectFnSig>(
		Key,
		WrapLuaFunction<EffectFnSig>(std::move(Fn), Key)
	);
}

void UTLoader::RegisterCurveFunction(const std::string& Key, sol::function Fn)
{
	UTFunctionRegistry::Instance().Register<CurveFnSig>(
		Key,
		WrapLuaFunction<CurveFnSig>(std::move(Fn), Key)
	);
}

void UTLoader::RegisterLuaTypes(sol::state& Lua)
{
	Lua.new_usertype<UTAgentContext>(
		"UTAgentContext",
		"GetNeed", &UTAgentContext::GetNeed,
		"SetNeed", &UTAgentContext::SetNeed,
		"GetStat", &UTAgentContext::GetStat,
		"SetStat", &UTAgentContext::SetStat
	);

	Lua.new_usertype<UTEffect>("UTEffect", sol::no_constructor);

	Lua.set_function("_RegisterScoreFunction", &UTLoader::RegisterScoreFunction);
	Lua.set_function("_RegisterEffectFunction", &UTLoader::RegisterEffectFunction);
	Lua.set_function("_RegisterCurveFunction", &UTLoader::RegisterCurveFunction);
}

UTEvaluationData UTLoader::LoadEvaluationData(const sol::table& Table, UTValidationResult& Result)
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

UTConsideration UTLoader::LoadConsideration(const sol::table& Table, UTValidationResult& Result)
{
	UTConsideration Consideration;
	LOAD_FIELD(Consideration, Key, Table, Result, true);

	if (const auto RawScoreFnKey = ValidateField<std::string>(Table, "RawScoreFnKey", Result))
	{
		Consideration.SetRawScoreFnKey(*RawScoreFnKey);
	}

	if (const auto ScoreCurveFnKey = ValidateField<std::string>(Table, "ScoreCurveFnKey", Result))
	{
		Consideration.SetScoreCurveFnKey(*ScoreCurveFnKey);
	}

	if (const auto Data = ValidateField<sol::table>(Table, "Data", Result))
	{
		Consideration.Data = LoadEvaluationData(*Data, Result);
	}

	return Consideration;
}

UTEffect UTLoader::LoadEffect(const sol::table& Table, UTValidationResult& Result)
{
	UTEffect Effect;
	LOAD_FIELD(Effect, Key, Table, Result, true);
	LOAD_FIELD(Effect, ConsiderationKey, Table, Result, false);
	LOAD_FIELD(Effect, bIsConsideration, Table, Result, false);

	if (const auto RawScoreFnKey = ValidateField<std::string>(Table, "RawScoreFnKey", Result))
	{
		Effect.SetRawScoreFnKey(*RawScoreFnKey);
	}

	if (const auto ScoreCurveFnKey = ValidateField<std::string>(Table, "ScoreCurveFnKey", Result))
	{
		Effect.SetScoreCurveFnKey(*ScoreCurveFnKey);
	}

	if (const auto EffectFnKey = ValidateField<std::string>(Table, "EffectFnKey", Result))
	{
		Effect.SetEffectFnKey(*EffectFnKey);
	}

	if (const auto Data = ValidateField<sol::table>(Table, "Data", Result))
	{
		Effect.Data = LoadEvaluationData(*Data, Result);
	}

	return Effect;
}

void UTLoader::ActionLoader(const sol::table& Table, const std::string& Category, UTValidationResult& Result)
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
			else if (Effect.is<UTEffect>())
			{
				Action.AddEffect(Effect.as<UTEffect>());
			}
			else
			{
				Result.AddError("Unexpected effect type in Effects array");
			}
		}
	}

	if (const auto Considerations = ValidateField<sol::table>(Table, "Considerations", Result))
	{
		for (auto& [_, Consideration] : *Considerations)
		{
			if (Consideration.get_type() == sol::type::table)
			{
				Action.Scorer.AddConsideration(LoadConsideration(Consideration, Result));
			}
		}
	}

	Action.GenerateConsiderations();
	UTActionRegistry::Instance().Register(Action, Category);
	LOG_INFO(std::format("Loaded Action: {} (Category: {})", Action.GetKey(), Category))
}

void UTLoader::GoalLoader(const sol::table& Table, const std::string& Category, UTValidationResult& Result)
{
	
}

std::optional<sol::table> UTLoader::LoadLuaTable(
	const std::filesystem::path& File,
	sol::state& Lua,
	UTValidationResult& Result)
{
	try
	{
		sol::load_result Script = Lua.load_file(File.string());
		if (!Script.valid())
		{
			Result.AddError(std::format("Failed to load script: {} Error: {}",
				File.string(), Script.get<sol::error>().what()));
			return std::nullopt;
		}

		sol::protected_function_result ScriptResult = Script();
		if (!ScriptResult.valid())
		{
			sol::error Err = ScriptResult;
			Result.AddError(std::format("Failed to execute script: {} Error: {}",
				File.string(), Err.what()));
			return std::nullopt;
		}

		if (ScriptResult.get_type() != sol::type::table)
		{
			Result.AddError(std::format("Script did not return a table: {}",
				File.string()));
			return std::nullopt;
		}

		return ScriptResult;
	}
	catch (const std::exception& Error)
	{
		Result.AddError(std::format("Exception loading script: {} Error: {}",
			File.string(), Error.what()));
		return std::nullopt;
	}
}

void UTLoader::LoadScriptsRecursive(
	const std::string& BaseDir,
	sol::state& Lua,
	UTValidationResult& Result,
	LoaderFn Loader)
{
	try
	{
		for (auto& Entry : std::filesystem::recursive_directory_iterator(BaseDir))
		{
			if (!Entry.is_regular_file() || Entry.path().extension() != ".lua") continue;

			// Category = relative folder from BaseDir
			std::filesystem::path Relative = std::filesystem::relative(Entry.path().parent_path(), BaseDir);
			std::string Category = Relative.empty() ? "Uncategorized" : Relative.string();

			if (auto Table = LoadLuaTable(Entry.path(), Lua, Result))
			{
				Loader(*Table, Category, Result);
			}
		}
	}
	catch (const std::exception& Error)
	{
		Result.AddError(std::format("Filesystem Error: {}", Error.what()));
	}
}