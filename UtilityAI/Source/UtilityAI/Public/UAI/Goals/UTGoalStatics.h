#pragma once
#include <vector>

namespace UAI
{
struct UTGoalScore
{
	const class UTGoal* Goal;
	float Score;

	bool operator>(const UTGoalScore& Other) const
	{
		return Score > Other.Score;
	}
};

std::vector<UTGoalScore> GetTopKGoalsWithScores(
	const std::vector<const UTGoal*>& GoalsToScore,
	const struct UTAgentContext& Context,
	int K = 1
);

}