#pragma once
#include "Logging/UTLogger.h"
#include <any>
#include <functional>
#include <string>

namespace UAI
{
class UTFunctionRegistry
{
public:
    static UTFunctionRegistry& Instance();

	template <typename Sig>
	void Register(const std::string& Key, std::function<Sig> Fn)
	{
		if (Key.empty())
		{
			LOG_WARN("[UTFunctionRegistry] Function key cannot be empty")
			return;
		}

		if (!Functions.contains(Key))
		{
			Functions[Key] = std::move(Fn);
		}
		else
		{
			LOG_WARN(std::format("[UTFunctionRegistry] Attempted to add duplicate function: '{}'", Key))
		}
	}

	template <typename Sig>
	const std::function<Sig>* Get(const std::string& Key) const 
	{
		if (Key.empty()) return nullptr;

		auto It = Functions.find(Key);
		if (It != Functions.end()) 
		{
			return std::any_cast<std::function<Sig>>(&It->second);
		}
		LOG_WARN(std::format("[UTFunctionRegistry] Function is not registered: '{}'", Key))
		return nullptr;
	}

	void Clear();

private:
	std::unordered_map<std::string, std::any> Functions;

	UTFunctionRegistry() = default;
	~UTFunctionRegistry();
	UTFunctionRegistry(const UTFunctionRegistry&) = delete;
	UTFunctionRegistry& operator=(const UTFunctionRegistry&) = delete;
	UTFunctionRegistry(UTFunctionRegistry&&) = delete;
	UTFunctionRegistry& operator=(UTFunctionRegistry&&) = delete;
};
}