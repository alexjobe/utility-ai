#include "UAI/UTGoal.h"
#include <UAI/UTFunctionRegistry.h>

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
	PreconditionFn = UTFunctionRegistry::Instance().Get<PreconditionFnSig>(InKey);
	PreconditionFnKey = PreconditionFn ? InKey : "";
}
