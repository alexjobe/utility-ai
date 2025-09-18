#pragma once
#include "UTAction.h"
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace UAI
{
class UTActionRegistry 
{
public:
	static UTActionRegistry& Instance() { static UTActionRegistry Singleton; return Singleton; }

	void Register(const UTAction& Action, const std::string& Category = "Uncategorized") 
	{
		Actions[Action.GetKey()] = Action;
		Categories[Category].insert(Action.GetKey());
	}

	UTAction* Get(const std::string& Key) 
	{
		auto It = Actions.find(Key);
		return It != Actions.end() ? &It->second : nullptr;
	}

	const std::unordered_set<std::string>& GetActionsInCategory(const std::string& Category) 
	{
		return Categories[Category];
	}

	void ClearCategory(const std::string& Category) 
	{
		if (Categories.contains(Category)) 
		{
			for (auto& Key : Categories[Category])
			{
				Actions.erase(Key);
			}
			Categories[Category].clear();
		}
	}

	void ClearAll() { Actions.clear(); Categories.clear(); }

private:
	std::unordered_map<std::string, UTAction> Actions;

	// Maps categories to actions
	std::unordered_map<std::string, std::unordered_set<std::string>> Categories;
};
}