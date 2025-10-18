#include "Scripting/UTLuaLoader.h"
#include "UAI/UTFunctionRegistry.h"
#include <Core/UTObjectRegistry.h>
#include <filesystem>
#include <UAI/UTAction.h>
#include <UAI/UTGoal.h>
#include <UAI/UTTrait.h>

using namespace UAI;
using namespace UTCore;;
using namespace UTLuaUtils;

void UTLuaLoader::RegisterScoreFunction(const std::string& Key, sol::function Fn)
{
	UTFunctionRegistry::Instance().Register<ScoreFnSig>(
		Key,
		WrapLuaFunction<ScoreFnSig>(std::move(Fn), Key)
	);
}

void UTLuaLoader::RegisterEffectFunction(const std::string& Key, sol::function Fn)
{
	UTFunctionRegistry::Instance().Register<EffectFnSig>(
		Key,
		WrapLuaFunction<EffectFnSig>(std::move(Fn), Key)
	);
}

void UTLuaLoader::RegisterCurveFunction(const std::string& Key, sol::function Fn)
{
	UTFunctionRegistry::Instance().Register<CurveFnSig>(
		Key,
		WrapLuaFunction<CurveFnSig>(std::move(Fn), Key)
	);
}

void UTLuaLoader::RegisterLuaTypes(sol::state& Lua)
{
	Lua.new_usertype<UTAgentContext>(
		"UTAgentContext",
		"GetNeed", &UTAgentContext::GetNeed,
		"SetNeed", &UTAgentContext::SetNeed,
		"GetStat", &UTAgentContext::GetStat,
		"SetStat", &UTAgentContext::SetStat
	);

	Lua.new_usertype<UTEffect>("UTEffect", sol::no_constructor);

	Lua.set_function("_RegisterScoreFunction", &UTLuaLoader::RegisterScoreFunction);
	Lua.set_function("_RegisterEffectFunction", &UTLuaLoader::RegisterEffectFunction);
	Lua.set_function("_RegisterCurveFunction", &UTLuaLoader::RegisterCurveFunction);
}

UTEvaluationData UTLuaLoader::LoadEvaluationData(const sol::table& Table, UTValidationResult& Result)
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

UTConsideration UTLuaLoader::LoadConsideration(const sol::table& Table, UTValidationResult& Result)
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

UTEffect UTLuaLoader::LoadEffect(const sol::table& Table, UTValidationResult& Result)
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

void UTLuaLoader::ActionLoader(const sol::table& Table, const std::string& Category, UTValidationResult& Result)
{
	UTAction Action;
	const auto Key = ValidateField<std::string>(Table, "Key", Result, true);
	if(!Key) return;

	Action.SetKey(*Key);

	LoadTags(Table, Result, [&](const std::string& Tag) { Action.Tags.insert(Tag); });
	LoadEffects(Table, Result, [&](const UTEffect& Effect) { Action.AddEffect(Effect); });
	LoadConsiderations(Table, Result, [&](const UTConsideration& Cons) { Action.Scorer.AddConsideration(Cons); });

	Action.GenerateConsiderations();
	UTObjectRegistry<UTAction>::Instance().Register(Action, Category);
	LOG_INFO(std::format("Loaded Action: {} (Category: {})", Action.GetKey(), Category))
}

void UTLuaLoader::GoalLoader(const sol::table& Table, const std::string& Category, UTValidationResult& Result)
{
	UTGoal Goal;
	const auto Key = ValidateField<std::string>(Table, "Key", Result, true);
	if (!Key) return;

	Goal.SetKey(*Key);

	LoadTags(Table, Result, [&](const std::string& Tag) { Goal.Tags.insert(Tag); });
	LoadConsiderations(Table, Result, [&](const UTConsideration& Cons) { Goal.Scorer.AddConsideration(Cons); });

	if (const auto PreconditionFnKey = ValidateField<std::string>(Table, "PreconditionFnKey", Result))
	{
		Goal.SetPreconditionFnKey(*PreconditionFnKey);
	}

	UTObjectRegistry<UTGoal>::Instance().Register(Goal, Category);
	LOG_INFO(std::format("Loaded Goal: {} (Category: {})", Goal.GetKey(), Category))
}

void UTLuaLoader::TraitLoader(const sol::table& Table, const std::string& Category, UTValidationResult& Result)
{
	UTTrait Trait;
	const auto Key = ValidateField<std::string>(Table, "Key", Result, true);
	if(!Key) return;

	Trait.SetKey(*Key);

	LoadTags(Table, Result, [&](const std::string& Tag) { Trait.Tags.insert(Tag); });
	LoadEffects(Table, Result, [&](const UTEffect& Effect) { Trait.AddEffect(Effect); });
	LoadConsiderations(Table, Result, [&](const UTConsideration& Cons) { Trait.AddConsideration(Cons); });

	UTObjectRegistry<UTTrait>::Instance().Register(Trait, Category);
	LOG_INFO(std::format("Loaded Trait: {} (Category: {})", Trait.GetKey(), Category))
}

std::optional<sol::table> UTLuaLoader::LoadLuaTable(
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

void UTLuaLoader::LoadScriptsRecursive(
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