#include "GWorld.h"
#include <utility>

using namespace Game;

GWorld& GWorld::Instance()
{
	static GWorld Singleton;
	return Singleton;
}

GCharacter* GWorld::AddCharacter(GCharacter&& NewChar)
{
	auto [It, bSuccess] = Characters.emplace(NewChar.GetKey(), std::move(NewChar));
	if (bSuccess)
	{
		return &It->second;
	}
	return nullptr;
}

Game::GCharacter* GWorld::GetCharacter(const std::string& Key)
{
	auto It = Characters.find(Key);
	if (It != Characters.end())
	{
		return &It->second;
	}
	return nullptr;
}
