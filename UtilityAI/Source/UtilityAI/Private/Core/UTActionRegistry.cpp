#include "Core/UTActionRegistry.h"
#include <Logging/Logger.h>

using namespace UAI;

UTActionRegistry::~UTActionRegistry()
{
	ClearAll();
}

UTActionRegistry& UTActionRegistry::Instance()
{
	static UTActionRegistry Singleton;
	return Singleton;
}

void UTActionRegistry::Register(const UTAction& Action, const std::string& Category)
{
	if (!Actions.contains(Action.GetKey()))
	{
		Actions[Action.GetKey()] = Action;
		Categories[Category].insert(Action.GetKey());
	}
	else
	{
		LOG_WARN(std::format("Attempted to add duplicate action to registry: {}", Action.GetKey()))
	}
}

UTAction* UTActionRegistry::Get(const std::string& Key)
{
	auto It = Actions.find(Key);
	if (It != Actions.end())
	{
		return &It->second;
	}
	LOG_WARN(std::format("Action is not registered: {}", Key))
	return nullptr;
}

const std::unordered_set<std::string>& UTActionRegistry::GetActionsInCategory(const std::string& Category)
{
	return Categories[Category];
}

void UTActionRegistry::ClearCategory(const std::string& Category)
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

void UTActionRegistry::ClearAll() 
{ 
	Actions.clear(); Categories.clear(); 
}
