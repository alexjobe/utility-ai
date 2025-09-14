#include "Core/UTGoal.h"
#include "Logging/Logger.h"

using namespace UtilityAI;

const bool UTGoal::AddConsideration(const UTConsideration& NewCons)
{
	if (NewCons.Key.empty())
	{
		LOG_ERROR(std::format("Goal: {} - Invalid Consideration! Check Key: {}", Name, NewCons.Key))
		return false;
	}

	auto It = Considerations.find(NewCons.Key);
	if (It != Considerations.end())
	{
		// Merge into existing consideration
		It->second.Data.Weight *= NewCons.Data.Weight;
		if (NewCons.ScoreCurve && NewCons.Data.Priority >= It->second.Data.Priority)
		{
			It->second.ScoreCurve = NewCons.ScoreCurve;
			It->second.Data.Priority = NewCons.Data.Priority;
		}
	}
	else
	{
		Considerations[NewCons.Key] = NewCons;
	}

	return true;
}

// Weighted geometric mean (log-sum)
float UTGoal::Evaluate(const UTAgentContext& Context) const
{
	if (PreconditionFn && !PreconditionFn(Context)) return 0.0f;
	if (Considerations.empty()) return 0.0f;

	float WeightedLogSum = 0.0f;
	float WeightSum = 0.0f;

	for (const auto& [Key, Cons] : Considerations)
	{
		const float ConScore = std::clamp(Cons.Score(Context), 0.001f, 1.0f);
		WeightedLogSum += Cons.Data.Weight * std::log(ConScore);
		WeightSum += Cons.Data.Weight;
	}

	return std::exp(WeightedLogSum / WeightSum);
}
