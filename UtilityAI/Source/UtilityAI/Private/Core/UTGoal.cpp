#include "Core/UTGoal.h"
#include <Core/UTFunctionRegistry.h>

using namespace UAI;

UTGoal::UTGoal(const std::string& InKey)
{
	SetKey(InKey);
}

void UTGoal::SetKey(const std::string& InKey)
{
	Key = InKey;
	Scorer.SetOwnerKey(InKey);
}

void UTGoal::SetPreconditionFnKey(const std::string& InKey)
{
	PreconditionFnKey = InKey;
	if (!PreconditionFnKey.empty())
	{
		PreconditionFn = UTFunctionRegistry::Instance().Get<PreconditionFnSig>(InKey);
	}
	else
	{
		PreconditionFn = nullptr;
	}
}
