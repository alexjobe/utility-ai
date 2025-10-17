#pragma once
#include "GCharacter.h"
#include <string>
#include <unordered_map>

namespace Game
{
	class GWorld
	{
	public:
		static GWorld& Instance();
		GCharacter* AddCharacter(GCharacter&& NewChar);
		GCharacter* GetCharacter(const std::string& Key);
		std::unordered_map<std::string, GCharacter>& GetAllCharacters() { return Characters; }

	private:
		std::unordered_map<std::string, GCharacter> Characters;

		GWorld() = default;
		~GWorld() = default;

		GWorld(const GWorld&) = delete;
		GWorld& operator=(const GWorld&) = delete;
		GWorld(GWorld&&) = delete;
		GWorld& operator=(GWorld&&) = delete;
	};
}