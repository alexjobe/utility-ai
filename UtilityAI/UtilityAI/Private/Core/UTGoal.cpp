#include "Core/UTGoal.h"

using namespace UtilityAI;

UTGoal::UTGoal(const std::string& InKey)
{
	Key = InKey;
	Scorer.SetOwnerKey(InKey);
}
