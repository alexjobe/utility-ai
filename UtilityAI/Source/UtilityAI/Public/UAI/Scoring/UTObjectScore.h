#pragma once
#include <memory>
#include <queue>
#include <vector>

namespace UAI
{
	template <typename T>
	struct UTObjectScore
	{
		std::unique_ptr<T> Object;
		float Score;

		bool operator>(const UTObjectScore<T>& Other) const
		{
			return Score > Other.Score;
		}
	};

	template <typename T>
	std::vector<UTObjectScore<T>> GetTopKWithScores(
		std::vector<T>&& ObjectsToScore,
		const struct UTAgentContext& Context,
		int K = 1
	)
	{
		if (K <= 0) return {};

		auto CompareFn = [](const UTObjectScore<T>& A, const UTObjectScore<T>& B) { return A > B; };

		std::priority_queue<UTObjectScore<T>, std::vector<UTObjectScore<T>>, decltype(CompareFn)> MinHeap(CompareFn);

		for (auto& Object : ObjectsToScore)
		{
			if (!Object.Scorer.PreconditionCheck(Context)) continue;

			const float Score = Object.Scorer.Score(Context);

			auto ScoredObject = UTObjectScore<T>(
				std::make_unique<T>(std::move(Object)),
				Score
			);

			if (MinHeap.size() < static_cast<size_t>(K))
			{
				MinHeap.push(std::move(ScoredObject));
			}
			else if (Score > MinHeap.top().Score)
			{
				MinHeap.pop();
				MinHeap.push(std::move(ScoredObject));
			}
		}

		// Extract results (unsorted)
		std::vector<UTObjectScore<T>> Result;
		Result.reserve(MinHeap.size());

		while (!MinHeap.empty())
		{
			// MinHeap top() is const&, hence the cast.
			Result.emplace_back(std::move(const_cast<UTObjectScore<T>&>(MinHeap.top())));
			MinHeap.pop();
		}

		// Sort in descending order (highest score first)
		std::sort(Result.begin(), Result.end(), CompareFn);

		return Result;
	}
}