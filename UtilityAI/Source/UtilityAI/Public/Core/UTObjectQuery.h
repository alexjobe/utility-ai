#pragma once
#include <functional>
#include <string>
#include <unordered_set>

namespace UTCore
{
template<typename T>
struct UTObjectQuery
{
	std::unordered_set<std::string> RequiredTags;
	std::unordered_set<std::string> ExcludedTags;

	std::function<bool(const T& Object)> PredicateFn; // Optional custom filter

	bool Matches(const T& Object) const
	{
		for (const auto& Tag : RequiredTags)
		{
			if (!T.Tags.contains(Tag))
			{
				return false;
			}
		}
		for (const auto& Tag : ExcludedTags)
		{
			if (T.Tags.contains(Tag))
			{
				return false;
			}
		}
		if (PredicateFn && !PredicateFn(T))
		{
			return false;
		}
	}
};
}