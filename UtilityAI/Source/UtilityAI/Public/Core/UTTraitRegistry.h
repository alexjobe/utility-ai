#pragma once
#include "UTTrait.h"
#include <unordered_set>

namespace UAI
{
class UTTraitRegistry
{
public:
	static UTTraitRegistry& Instance();

	void Register(const UTTrait& Trait, const std::string& Category = "Uncategorized");

	UTTrait* Get(const std::string& Key);
	const std::unordered_set<std::string>& GetTraitsInCategory(const std::string& Category);
	const std::unordered_map<std::string, UTTrait>& GetTraits() const { return Traits; }

	void ClearCategory(const std::string& Category);
	void ClearAll();

private:
	std::unordered_map<std::string, UTTrait> Traits;

	// Maps categories to traits
	std::unordered_map<std::string, std::unordered_set<std::string>> Categories;
};
}