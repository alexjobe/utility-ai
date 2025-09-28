#include "Core/UTGoal.h"

using namespace UAI;

UTGoal::UTGoal(const std::string& InKey)
{
	Key = InKey;
	Scorer.SetOwnerKey(InKey);
}
