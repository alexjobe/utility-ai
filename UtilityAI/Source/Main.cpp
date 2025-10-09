#include "Core/UTAction.h"
#include "Game/Character.h"
#include "Game/GameHelpers.h"
#include <Core/UTEffectTypes.h>
#include <Core/UTObjectRegistry.h>
#include <Editor/UTActionsPanel.h>
#include <Editor/UTGoalsPanel.h>
#include <Editor/UTTraitsPanel.h>
#include <Logging/Logger.h>
#include <Scripting/UTLuaLoader.h>
#include <Scripting/UTLuaLogger.h>
#include <sol.hpp>
#include <UIEditorApp.h>
#define SOL_ALL_SAFETIES_ON 1

using namespace UAI;
using namespace Game;
using namespace Log;
using namespace UI;

int main()
{
	Logger::Instance().AddSink(std::make_shared<ConsoleSink>(EVerbosity::Minimal));

	UAI::RegisterEffectTypes();

	sol::state Lua;
	Lua.open_libraries(sol::lib::base);
	LuaLog::RegisterLogger(Lua);
	UTLoader::RegisterLuaTypes(Lua);
	Game::RegisterLuaTypes(Lua);

	UTValidationResult Result;
	UTLoader::LoadScriptsRecursive("Scripts/Actions", Lua, Result, UTLoader::ActionLoader);
	UTLoader::LoadScriptsRecursive("Scripts/Goals", Lua, Result, UTLoader::GoalLoader);
	UTLoader::LoadScriptsRecursive("Scripts/Traits", Lua, Result, UTLoader::TraitLoader);
	if (!Result.bValid)
	{
		for (const auto& Error : Result.Errors)
		{
			LOG_ERROR(Error)
		}
	}

	Character MyCharacter;
	MyCharacter.CoreStats[ECoreStatType::Strength] = 11.f;
	MyCharacter.CoreStats[ECoreStatType::Endurance] = 7.f;

	UTAgentContext MyContext = MyCharacter.CreateUtilityContext();

	if (UTAction* TestAction = UTObjectRegistry<UTAction>::Instance().Get("Action.Raid"))
	{
		TestAction->Scorer.Score(MyContext);
		TestAction->Execute(MyContext);
	}

	UIEditorApp App;
	if (!App.Init())
	{
		return -1;
	}

	App.GetWindowManager().AddPanel(std::make_unique<UTEditor::UTActionsPanel>());
	App.GetWindowManager().AddPanel(std::make_unique<UTEditor::UTGoalsPanel>());
	App.GetWindowManager().AddPanel(std::make_unique<UTEditor::UTTraitsPanel>());

	App.Run();

	UTObjectRegistry<UTAction>::Instance().ClearAll();
	UTFunctionRegistry::Instance().Clear();

	return 0;
}