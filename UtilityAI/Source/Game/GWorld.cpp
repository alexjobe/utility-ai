#include "GWorld.h"

using namespace Game;

GWorld& GWorld::Instance()
{
	static GWorld Singleton;
	return Singleton;
}

void GWorld::AddCharacter(const GCharacter& NewChar)
{
	if (!Characters.contains(NewChar.Key))
	{
		Characters[NewChar.Key] = NewChar;
	}
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
