#pragma once

#include <string>

namespace Game
{
	constexpr float MIN_NEED = 0.f;
	constexpr float MAX_NEED = 100.f;

	enum class ENeedType
	{
		Survival,
		Social,
		Wealth,
		Status,
		Curiosity,
		Faith
	};

	inline std::string ToString(const ENeedType& InType)
	{
		switch (InType)
		{
		case ENeedType::Survival:
			return "Survival";
		case ENeedType::Social:
			return "Social";
		case ENeedType::Wealth:
			return "Wealth";
		case ENeedType::Status:
			return "Status";
		case ENeedType::Curiosity:
			return "Curiosity";
		case ENeedType::Faith:
			return "Faith";
		default:
			return "INVALID";
		}
	}

	constexpr float MIN_CORE_STAT = 0.f;
	constexpr float MAX_CORE_STAT = 20.f;

	enum class ECoreStatType
	{
		Strength,
		Agility,
		Endurance,
		Intellect,
		Willpower,
		Charisma
	};

	inline std::string ToString(const ECoreStatType& InType)
	{
		switch (InType)
		{
		case ECoreStatType::Strength:
			return "Strength";
		case ECoreStatType::Agility:
			return "Agility";
		case ECoreStatType::Endurance:
			return "Endurance";
		case ECoreStatType::Intellect:
			return "Intellect";
		case ECoreStatType::Willpower:
			return "Willpower";
		case ECoreStatType::Charisma:
			return "Charisma";
		default:
			return "INVALID";
		}
	}
}