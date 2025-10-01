#include "Core/UTAction.h"
#include "Game/Character.h"
#include "Game/GameHelpers.h"
#include <Core/UTEffectTypes.h>
#include <Logging/Logger.h>
#include <Scripting/UTLuaLoader.h>
#include <Scripting/UTLuaLogger.h>
#include <sol.hpp>
#include <UIEditorApp.h>
#include <Editor/UTActionsPanel.h>
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
	UAI::RegisterLuaTypes(Lua);
	Game::RegisterLuaTypes(Lua);

	UTValidationResult Result;
	LoadActionsRecursive("Scripts/Actions", Lua, Result);
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

	if (UTAction* TestAction = UTActionRegistry::Instance().Get("Action.Raid"))
	{
		TestAction->Scorer.Score(MyContext);
		TestAction->Execute(MyContext);
	}

	UIEditorApp App;
	if (!App.Init())
	{
		return -1;
	}

	App.GetWindowManager().AddPanel(std::make_unique<UTActionsPanel>());

	App.Run();

	UTActionRegistry::Instance().ClearAll();
	UTFunctionRegistry::Instance().Clear();

	return 0;
}