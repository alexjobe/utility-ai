#pragma once
#include <Logging/Logger.h>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace UAI
{
template<typename T>
class UTObjectRegistry
{
public:
	static UTObjectRegistry& Instance()
	{
		static UTObjectRegistry Singleton;
		return Singleton;
	}

	void Register(const T& Object, const std::string& Category = "Uncategorized")
	{
		if (!Objects.contains(Object.GetKey()))
		{
			Objects[Object.GetKey()] = Object;
			Categories[Category].insert(Object.GetKey());
		}
		else
		{
			LOG_WARN(std::format("Attempted to add duplicate object to registry: {}", Object.GetKey()))
		}
	}

	T* Get(const std::string& Key)
	{
		auto It = Objects.find(Key);
		if (It != Objects.end())
		{
			return &It->second;
		}
		LOG_WARN(std::format("Object is not registered: {}", Key))
		return nullptr;
	}

	const std::unordered_set<std::string>& GetInCategory(const std::string& Category)
	{
		static const std::unordered_set<std::string> Empty;
		auto It = Categories.find(Category);
		return (It != Categories.end()) ? It->second : Empty;
	}

	const std::unordered_set<std::string> FindAllWithTag(const std::string& Tag) const
	{
		std::unordered_set<std::string> Result;
		for (const auto& [_, Object] : Objects)
		{
			if (Object.Tags.contains(Tag))
			{
				Result.insert(Object.GetKey());
			}
		}
		return Result;
	}

	const std::unordered_set<std::string> FindAllWithTags(const std::vector<std::string>& Tags) const
	{
		std::unordered_set<std::string> Result;
		for (const auto& [_, Object] : Objects)
		{
			bool bHasTags = true;
			for (const auto& Tag : Tags)
			{
				if (!Object.Tags.contains(Tag))
				{
					bHasTags = false;
					break;
				}
			}
			if (bHasTags)
			{
				Result.insert(Object.GetKey());
			}
		}
		return Result;
	}

	const std::unordered_set<std::string> FindAllWithAnyTag(const std::vector<std::string>& Tags) const
	{
		std::unordered_set<std::string> Result;
		for (const auto& [_, Object] : Objects)
		{
			for (const auto& Tag : Tags)
			{
				if (Object.Tags.contains(Tag))
				{
					Result.insert(Object.GetKey());
					break;
				}
			}
		}
		return Result;
	}

	const std::unordered_map<std::string, T>& GetAll() const { return Objects; }

	void ClearCategory(const std::string& Category)
	{
		if (Categories.contains(Category))
		{
			for (auto& Key : Categories[Category])
			{
				Objects.erase(Key);
			}
			Categories[Category].clear();
		}
	}

	void ClearAll()
	{
		Objects.clear(); Categories.clear();
	}

private:
	std::unordered_map<std::string, T> Objects;
	std::unordered_map<std::string, std::unordered_set<std::string>> Categories;

	UTObjectRegistry() = default;
	~UTObjectRegistry() = default;

	UTObjectRegistry(const UTObjectRegistry&) = delete;
	UTObjectRegistry& operator=(const UTObjectRegistry&) = delete;
	UTObjectRegistry(UTObjectRegistry&&) = delete;
	UTObjectRegistry& operator=(UTObjectRegistry&&) = delete;
};
}