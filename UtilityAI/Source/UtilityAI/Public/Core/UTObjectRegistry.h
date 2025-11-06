#pragma once
#include "UTObjectQuery.h"
#include <Logging/UTLogger.h>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
#include <vector>

namespace UTCore
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

	// Register a new object into the registry
	void Register(std::unique_ptr<T> Object, const std::string& Category = "Uncategorized")
	{
		if (Object == nullptr)
		{
			LOG_WARN("[UTObjectRegistry] Attempted to register null object!")
			return;
		}

		const auto& Key = Object->GetKey();

		if (Objects.contains(Key))
		{
			LOG_WARN(std::format("[UTObjectRegistry] Attempted to add duplicate object to registry: '{}'", Object->GetName()))
			return;
		}

		Categories[Category].insert(Key);
		NameIndex[Object->GetName()].insert(Key);

		// Build TagIndex for fast filtering
		for (const auto& Tag : Object->OwnedTags)
		{
			TagIndex[Tag].insert(Key);
		}

		// Move ownership into the map
		Objects.emplace(Key, std::move(Object));
	}

	// Retrieve object (mutable)
	T* Get(const std::string& Key)
	{
		auto It = Objects.find(Key);
		if (It != Objects.end())
		{
			return It->second.get();
		}

		LOG_WARN(std::format("[UTObjectRegistry] Object with key not found in registry: '{}'", Key))
		return nullptr;
	}

	// Retrieve object (const)
	const T* Get(const std::string& Key) const
	{
		auto It = Objects.find(Key);
		return (It != Objects.end()) ? It->second.get() : nullptr;
	}

	// Get all objects in a category
	const std::unordered_set<std::string>& GetAllInCategory(const std::string& Category) const
	{
		static const std::unordered_set<std::string> Empty;
		auto It = Categories.find(Category);
		return (It != Categories.end()) ? It->second : Empty;
	}

	// General-purpose query
	std::vector<const T*> Query(const UTObjectQuery<T>& QueryDef) const
	{
		std::vector<const T*> Results;

		// Fast path: if RequiredTags has at least one tag, start from TagIndex
		if (!QueryDef.RequiredTags.empty())
		{
			// Start with objects matching the first required tag
			std::string FirstTag = *QueryDef.RequiredTags.begin();
			auto TagIt = TagIndex.find(FirstTag);
			if (TagIt == TagIndex.end()) return Results;

			// Copy initial candidates
			for (const auto& Key : TagIt->second)
			{
				const auto* Obj = Get(Key);
				if (Obj && QueryDef.Matches(*Obj))
				{
					Results.push_back(Obj);
				}
			}
		}
		else
		{
			// Fall back to scanning all objects
			for (const auto& [_, Obj] : Objects)
			{
				if (QueryDef.Matches(*Obj))
				{
					Results.push_back(Obj.get());
				}
			}
		}

		return Results;
	}

	// Get all registered objects (read-only)
	std::vector<const T*> GetAll() const 
	{ 
		std::vector<const T*> Results;

		for (const auto& [_, Obj] : Objects)
		{
			Results.push_back(Obj.get());
		}

		return Results;
	}

	std::vector<const T*> FindAllWithName(const std::string& Name)
	{
		std::vector<const T*> Results;
		if (auto It = NameIndex.find(Name); It != NameIndex.end())
		{
			for (const auto& Key : It->second)
			{
				const auto* Obj = Get(Key);
				Results.push_back(Obj);
			}
		}

		if (Results.empty())
		{
			LOG_WARN(std::format("[UTObjectRegistry] Object with name not found in registry: '{}'", Name))
		}

		return Results;
	}

	std::vector<const T*> FindAllWithTag(const std::string& Tag) const
	{
		UTObjectQuery<T> QueryDef;
		QueryDef.RequiredTags = { Tag };
		return Query(QueryDef);
	}

	std::vector<const T*> FindAllWithAnyTag(const std::unordered_set<std::string>& Tags) const
	{
		UTObjectQuery<T> QueryDef;
		QueryDef.AnyTags = Tags;
		return Query(QueryDef);
	}

	// Remove everything in a category
	void ClearCategory(const std::string& Category)
	{
		auto It = Categories.find(Category);
		if (It == Categories.end())
		{
			return;
		}

		for (const auto& Key : It->second)
		{
			auto ObjIt = Objects.find(Key);
			if (ObjIt != Objects.end())
			{
				// Remove from tag index
				for (const auto& Tag : ObjIt->second.OwnedTags)
				{
					if (auto TagIt = TagIndex.find(Tag); TagIt != TagIndex.end())
					{
						TagIt->second.erase(Key);
					}
				}

				Objects.erase(ObjIt);
			}
		}

		It->second.clear();
	}

	// Clear all objects from registry
	void ClearAll()
	{
		Objects.clear();
		Categories.clear();
		TagIndex.clear();
	}

private:
	std::unordered_map<std::string, std::unique_ptr<T>> Objects;
	std::unordered_map<std::string, std::unordered_set<std::string>> Categories;
	std::unordered_map<std::string, std::unordered_set<std::string>> NameIndex;
	std::unordered_map<std::string, std::unordered_set<std::string>> TagIndex;

	UTObjectRegistry() = default;
	~UTObjectRegistry() = default;

	UTObjectRegistry(const UTObjectRegistry&) = delete;
	UTObjectRegistry& operator=(const UTObjectRegistry&) = delete;
	UTObjectRegistry(UTObjectRegistry&&) = delete;
	UTObjectRegistry& operator=(UTObjectRegistry&&) = delete;
};
}