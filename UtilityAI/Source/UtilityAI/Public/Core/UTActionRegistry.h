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
	static UTActionRegistry& Instance();

	void Register(const UTAction& Action, const std::string& Category = "Uncategorized");

	UTAction* Get(const std::string& Key);

	const std::unordered_set<std::string>& GetActionsInCategory(const std::string& Category);

	const std::unordered_map<std::string, UTAction>& GetActions() const { return Actions; }

	void ClearCategory(const std::string& Category);

	void ClearAll();

private:
	std::unordered_map<std::string, UTAction> Actions;

	// Maps categories to actions
	std::unordered_map<std::string, std::unordered_set<std::string>> Categories;

	UTActionRegistry() = default;
	~UTActionRegistry();
	UTActionRegistry(const UTActionRegistry&) = delete;
	UTActionRegistry& operator=(const UTActionRegistry&) = delete;
	UTActionRegistry(UTActionRegistry&&) = delete;
	UTActionRegistry& operator=(UTActionRegistry&&) = delete;
};
}