#include "Scripting/UTLuaLoader.h"
#include "UTFunctionRegistry.h"
#include <Core/UTObjectRegistry.h>
#include <filesystem>
#include <memory>
#include <UTAction.h>
#include <UTGoal.h>
#include <UTTrait.h>

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

UTEvaluationData UTLuaLoader::LoadEvaluationData(const sol::table& Table, UTScriptResult& Result)
{
	UTEvaluationData Data;
	LOAD_FIELD(Data, Target, Table, Result, true);
	LOAD_FIELD(Data, Raw, Table, Result, false);
	LOAD_FIELD(Data, MinRaw, Table, Result, false);
	LOAD_FIELD(Data, MaxRaw, Table, Result, false);
	LOAD_FIELD(Data, BaseWeight, Table, Result, false);
	LOAD_FIELD(Data, Priority, Table, Result, false);
	return Data;
}

UTConsideration UTLuaLoader::LoadConsideration(const sol::table& Table, UTScriptResult& Result)
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

UTEffect UTLuaLoader::LoadEffect(const sol::table& Table, UTScriptResult& Result)
{
	UTEffect Effect;
	LOAD_FIELD(Effect, Key, Table, Result, true);
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

UAI::UTBias UTLuaLoader::LoadBias(const sol::table& Table, UTScriptResult& Result)
{
	UTBias Bias;
	LOAD_FIELD(Bias, Target, Table, Result, true);
	LOAD_FIELD(Bias, WeightMultiplier, Table, Result, false);
	return Bias;
}

void UTLuaLoader::ActionLoader(const sol::table& Table, UTScriptResult& Result)
{
	const auto Name = ValidateField<std::string>(Table, "Name", Result, true);
	if(!Name) return;

	auto Action = std::make_unique<UTAction>();
	Action->SetName(*Name);

	LoadTags(Table, "OwnedTags", Result, [&](const std::string& Tag) { Action->OwnedTags.insert(Tag); });
	LoadEffects(Table, Result, [&](const UTEffect& Effect) { Action->AddEffect(Effect); });
	LoadConsiderations(Table, Result, [&](const UTConsideration& Cons) { Action->Scorer.AddConsideration(Cons); });

	if (const auto PreconditionFnKey = ValidateField<std::string>(Table, "PreconditionFnKey", Result))
	{
		Action->Scorer.SetPreconditionFnKey(*PreconditionFnKey);
	}

	Action->GenerateConsiderations();

	UTObjectRegistry<UTAction>::Instance().Register(std::move(Action), Result.Category);
	LOG_INFO(std::format("[UTLuaLoader] Loaded Action: '{}' (Category: {})", *Name, Result.Category))
}

void UTLuaLoader::GoalLoader(const sol::table& Table, UTScriptResult& Result)
{
	const auto Name = ValidateField<std::string>(Table, "Name", Result, true);
	if (!Name) return;

	auto Goal = std::make_unique<UTGoal>();
	Goal->SetName(*Name);

	LoadTags(Table, "OwnedTags", Result, [&](const std::string& Tag) { Goal->OwnedTags.insert(Tag); });
	LoadTags(Table, "RequiredTags", Result, [&](const std::string& Tag) { Goal->RequiredTags.insert(Tag); });
	LoadConsiderations(Table, Result, [&](const UTConsideration& Cons) { Goal->Scorer.AddConsideration(Cons); });

	if (const auto PreconditionFnKey = ValidateField<std::string>(Table, "PreconditionFnKey", Result))
	{
		Goal->Scorer.SetPreconditionFnKey(*PreconditionFnKey);
	}

	UTObjectRegistry<UTGoal>::Instance().Register(std::move(Goal), Result.Category);
	LOG_INFO(std::format("[UTLuaLoader] Loaded Goal: '{}' (Category: {})", *Name, Result.Category))
}

void UTLuaLoader::TraitLoader(const sol::table& Table, UTScriptResult& Result)
{
	const auto Name = ValidateField<std::string>(Table, "Name", Result, true);
	if(!Name) return;

	auto Trait = std::make_unique<UTTrait>();
	Trait->SetName(*Name);

	LoadTags(Table, "OwnedTags", Result, [&](const std::string& Tag) { Trait->OwnedTags.insert(Tag); });
	LoadBiases(Table, Result, [&](const UTBias& Bias) { Trait->AddBias(Bias); });
	LoadEffects(Table, Result, [&](const UTEffect& Effect) { Trait->AddEffect(Effect); });

	UTObjectRegistry<UTTrait>::Instance().Register(std::move(Trait), Result.Category);
	LOG_INFO(std::format("[UTLuaLoader] Loaded Trait: '{}' (Category: {})", *Name, Result.Category))
}

void UTLuaLoader::LoadAllScripts(sol::state& Lua)
{
	std::vector<UTScriptResult> Results;
	bool bSuccess = true;
	bSuccess &= UTLuaLoader::LoadScriptsRecursive("Scripts/Actions", Lua, Results, UTLuaLoader::ActionLoader);
	bSuccess &= UTLuaLoader::LoadScriptsRecursive("Scripts/Goals", Lua, Results, UTLuaLoader::GoalLoader);
	bSuccess &= UTLuaLoader::LoadScriptsRecursive("Scripts/Traits", Lua, Results, UTLuaLoader::TraitLoader);
	if (!bSuccess)
	{
		for (const auto& Result : Results)
		{
			Result.Log();
		}
	}
}

bool UTLuaLoader::LoadScriptsRecursive(
	const std::string& BaseDir,
	sol::state& Lua,
	std::vector<UTScriptResult>& Results,
	LoaderFn Loader)
{
	bool bSuccess = true;
	try
	{
		for (auto& Entry : std::filesystem::recursive_directory_iterator(BaseDir))
		{
			if (!Entry.is_regular_file() || Entry.path().extension() != ".lua") continue;

			UTScriptResult Result;

			Result.FilePath = Entry.path().string();
			Result.FileName = Entry.path().filename().string();

			// Category = relative folder from BaseDir
			std::filesystem::path Relative = std::filesystem::relative(Entry.path().parent_path(), BaseDir);
			Result.Category = Relative.empty() ? "Uncategorized" : Relative.string();

			if (auto Table = LoadLuaTable(Lua, Result))
			{
				Loader(*Table, Result);
			}

			bSuccess &= Result.bValid;
			Results.push_back(Result);
		}
	}
	catch (const std::exception& Error)
	{
		UTScriptResult Result;
		Result.AddError(std::format("Filesystem Error: {}", Error.what()));
		Results.push_back(Result);
		bSuccess = false;
	}

	return bSuccess;
}

std::optional<sol::table> UTLuaLoader::LoadLuaTable(
	sol::state& Lua,
	UTScriptResult& Result)
{
	try
	{
		sol::load_result Script = Lua.load_file(Result.FilePath);
		if (!Script.valid())
		{
			Result.AddError(std::format("Failed to load script. Error: {}", Script.get<sol::error>().what()));
			return std::nullopt;
		}

		sol::protected_function_result ScriptResult = Script();
		if (!ScriptResult.valid())
		{
			sol::error Err = ScriptResult;
			Result.AddError(std::format("Failed to execute script. Error: {}", Err.what()));
			return std::nullopt;
		}

		if (ScriptResult.get_type() != sol::type::table)
		{
			Result.AddError("Script did not return a table");
			return std::nullopt;
		}

		return ScriptResult;
	}
	catch (const std::exception& Error)
	{
		Result.AddError(std::format("Exception loading script. Error: {}", Error.what()));
		return std::nullopt;
	}
}