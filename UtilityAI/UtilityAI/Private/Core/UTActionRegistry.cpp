#include "Core/UTActionRegistry.h"

using namespace UAI;

UTActionRegistry& UTActionRegistry::Instance()
{
	static UTActionRegistry Singleton;
	return Singleton;
}

void UTActionRegistry::Register(const UTAction& Action, const std::string& Category)
{
	Actions[Action.GetKey()] = Action;
	Categories[Category].insert(Action.GetKey());
}

UTAction* UTActionRegistry::Get(const std::string& Key)
{
	auto It = Actions.find(Key);
	return It != Actions.end() ? &It->second : nullptr;
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
