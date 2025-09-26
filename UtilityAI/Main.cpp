#define SOL_ALL_SAFETIES_ON 1

#include "Core/UTAction.h"
#include "Core/UTAgentContext.h"
#include "Core/UTScorer.h"
#include "Game/Character.h"
#include "Game/GameHelpers.h"
#include "Game/StatTypes.h"
#include "Logging/Logger.h"
#include <Core/UTActionRegistry.h>
#include <memory>
#include <Scripting/UTLuaLoader.h>
#include <Scripting/UTLuaLogger.h>
#include <sol/sol.hpp>
#include <string>

using namespace UAI;
using namespace Game;
using namespace Log;

int main()
{
	Logger::Instance().AddSink(std::make_shared<ConsoleSink>(EVerbosity::Minimal));

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
}