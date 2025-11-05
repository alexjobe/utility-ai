#pragma once
#include <vector>

namespace UAI
{
	template <typename T>
	struct UTObjectScore
	{
		const T* Object;
		float Score;

		bool operator>(const UTObjectScore<T>& Other) const
		{
			return Score > Other.Score;
		}
	};

	template <typename T>
	std::vector<UTObjectScore<T>> GetTopKWithScores(
		const std::vector<const T*>& ObjectsToScore,
		const struct UTAgentContext& Context,
		int K = 1
	)
	{
		if (K <= 0) return {};

		auto CompareFn = [](const UTObjectScore<T>& A, const UTObjectScore<T>& B) { return A > B; };

		std::priority_queue<UTObjectScore<T>, std::vector<UTObjectScore<T>>, decltype(CompareFn)> MinHeap(CompareFn);

		for (const auto* Object : ObjectsToScore)
		{
			if (Object == nullptr)
			{
				LOG_ERROR("UAI::GetTopKWithScores: Object cannot be null!")
				continue;
			}

			if (!Object->Scorer.PreconditionCheck(Context)) continue;

			const float Score = Object->Scorer.Score(Context);

			if (MinHeap.size() < static_cast<size_t>(K))
			{
				MinHeap.push({ Object, Score });
			}
			else if (Score > MinHeap.top().Score)
			{
				MinHeap.pop();
				MinHeap.push({ Object, Score });
			}
		}

		// Extract results (unsorted)
		std::vector<UTObjectScore<T>> Result;
		Result.reserve(MinHeap.size());
		while (!MinHeap.empty())
		{
			Result.push_back(MinHeap.top());
			MinHeap.pop();
		}

		return Result;
	}
}