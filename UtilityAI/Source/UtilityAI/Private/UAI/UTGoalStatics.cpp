#include "UAI/UTGoalStatics.h"
#include <Logging/UTLogger.h>
#include <queue>
#include <UAI/UTAgentContext.h>
#include <UAI/UTGoal.h>

using namespace UAI;

std::vector<UTGoalScore> UAI::GetTopKGoalsWithScores(
	const std::vector<const UTGoal*>& GoalsToScore,
	const UTAgentContext& Context,
	int K
)
{
	if(K <= 0) return {};

	auto CompareFn = [](const UTGoalScore& A, const UTGoalScore& B) { return A > B; };

	std::priority_queue<UTGoalScore, std::vector<UTGoalScore>, decltype(CompareFn)> MinHeap(CompareFn);

	for (const auto* Goal : GoalsToScore)
	{
		if (Goal == nullptr)
		{
			LOG_ERROR("UAI::GetTopKGoalsWithScores: Goal cannot be null!")
			continue;
		}

		if (!Goal->PreconditionCheck(Context))
			continue;

		const float Score = Goal->Scorer.Score(Context);

		if (MinHeap.size() < static_cast<size_t>(K))
		{
			MinHeap.push({ Goal, Score });
		}
		else if (Score > MinHeap.top().Score)
		{
			MinHeap.pop();
			MinHeap.push({ Goal, Score });
		}
	}

	// Extract results (unsorted)
	std::vector<UTGoalScore> Result;
	Result.reserve(MinHeap.size());
	while (!MinHeap.empty())
	{
		Result.push_back(MinHeap.top());
		MinHeap.pop();
	}

	return Result;
}

