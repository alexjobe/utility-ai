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
	std::unordered_set<std::string> AnyTags;

	std::function<bool(const T&)> PredicateFn; // Optional custom filter

	bool Matches(const T& Object) const
	{
		// Must have all required tags
		for (const auto& Tag : RequiredTags)
		{
			if (!Object.OwnedTags.contains(Tag))
			{
				return false;
			}
		}

		// Must NOT have excluded tags
		for (const auto& Tag : ExcludedTags)
		{
			if (Object.OwnedTags.contains(Tag))
			{
				return false;
			}
		}

		// Must have at least one of the "Any" tags (if any are specified)
		if (!AnyTags.empty())
		{
			bool bHasAny = false;
			for (const auto& Tag : AnyTags)
			{
				if (Object.OwnedTags.contains(Tag))
				{
					bHasAny = true;
					break;
				}
			}
			if (!bHasAny)
			{
				return false;
			}
		}

		// Optional predicate
		if (PredicateFn && !PredicateFn(Object))
		{
			return false;
		}

		return true;
	}
};
}