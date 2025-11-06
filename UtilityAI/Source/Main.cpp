#include "Game/GameHelpers.h"
#include "Game/GCharacter.h"
#include "Game/GCharactersPanel.h"
#include "Game/GWorld.h"
#include "UTAction.h"
#include <Core/UTObjectRegistry.h>
#include <Editor/UTActionsPanel.h>
#include <Editor/UTEditorApp.h>
#include <Editor/UTGoalsPanel.h>
#include <Editor/UTTraitsPanel.h>
#include <Logging/UTLogger.h>
#include <Scripting/UTLuaLoader.h>
#include <Scripting/UTLuaLogger.h>
#include <sol.hpp>
#include <UTEffectTypes.h>
#define SOL_ALL_SAFETIES_ON 1

using namespace UAI;
using namespace Game;
using namespace UTCore;
using namespace UTLog;
using namespace UTEditor;

int main()
{
	Logger::Instance().AddSink(std::make_shared<ConsoleSink>(EVerbosity::Minimal));

	UAI::RegisterEffectTypes();

	sol::state Lua;
	Lua.open_libraries(sol::lib::base);
	UTLuaLog::RegisterLogger(Lua);
	UTLuaLoader::RegisterLuaTypes(Lua);
	Game::RegisterLuaTypes(Lua);

	UTValidationResult Result;
	UTLuaLoader::LoadScriptsRecursive("Scripts/Actions", Lua, Result, UTLuaLoader::ActionLoader);
	UTLuaLoader::LoadScriptsRecursive("Scripts/Goals", Lua, Result, UTLuaLoader::GoalLoader);
	UTLuaLoader::LoadScriptsRecursive("Scripts/Traits", Lua, Result, UTLuaLoader::TraitLoader);
	if (!Result.bValid)
	{
		for (const auto& Error : Result.Errors)
		{
			LOG_ERROR(Error)
		}
	}

	UTAgentContext MyContext;
	if (GCharacter* MyCharacter = GWorld::Instance().AddCharacter(GCharacter("Bilbo")))
	{
		MyCharacter->Profession = "Burglar";
		MyCharacter->CoreStats[ECoreStatType::Strength] = 11.f;
		MyCharacter->CoreStats[ECoreStatType::Endurance] = 7.f;
		MyContext = MyCharacter->CreateAgentContext();
		MyCharacter->UpdateGoals();
		MyCharacter->UpdateActions();
	}

	UTEditorApp App;
	if (!App.Init())
	{
		return -1;
	}

	App.GetWindowManager().AddPanel(std::make_unique<UTEditor::UTActionsPanel>());
	App.GetWindowManager().AddPanel(std::make_unique<UTEditor::UTGoalsPanel>());
	App.GetWindowManager().AddPanel(std::make_unique<UTEditor::UTTraitsPanel>());
	App.GetWindowManager().AddPanel(std::make_unique<GCharactersPanel>());

	App.Run();

	UTObjectRegistry<UTAction>::Instance().ClearAll();
	UTFunctionRegistry::Instance().Clear();

	return 0;
}