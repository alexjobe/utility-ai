#include "Core/UTTraitRegistry.h"
#include <Logging/Logger.h>

using namespace UAI;

UTTraitRegistry& UTTraitRegistry::Instance()
{
	static UTTraitRegistry Singleton;
	return Singleton;
}

void UTTraitRegistry::Register(const UTTrait& Trait, const std::string& Category /*= "Uncategorized"*/)
{
	if (!Traits.contains(Trait.Key))
	{
		Traits[Trait.Key] = Trait;
		Categories[Category].insert(Trait.Key);
	}
	else
	{
		LOG_WARN(std::format("Attempted to add duplicate trait to registry: '{}'", Trait.Key))
	}
}

UTTrait* UTTraitRegistry::Get(const std::string& Key)
{
	auto It = Traits.find(Key);
	if (It != Traits.end())
	{
		return &It->second;
	}
	LOG_WARN(std::format("Trait is not registered: '{}'", Key))
	return nullptr;
}

const std::unordered_set<std::string>& UTTraitRegistry::GetTraitsInCategory(const std::string& Category)
{
	return Categories[Category];
}

void UTTraitRegistry::ClearCategory(const std::string& Category)
{
	if (Categories.contains(Category))
	{
		for (auto& Key : Categories[Category])
		{
			Traits.erase(Key);
		}
		Categories[Category].clear();
	}
}

void UTTraitRegistry::ClearAll()
{
	Traits.clear(); Categories.clear();
}

