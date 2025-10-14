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
		void AddCharacter(const GCharacter& NewChar);
		GCharacter* GetCharacter(const std::string& Key);
		const std::unordered_map<std::string, GCharacter>& GetAllCharacters() const { return Characters; }

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