#include <UTFunctionRegistry.h>
#include <UTGoal.h>

using namespace UAI;

UTGoal::UTGoal()
{
	Scorer.SetOwner(this);
}

void UTGoal::SetPreconditionFnKey(const std::string& InKey)
{
	PreconditionFn = UTFunctionRegistry::Instance().Get<PreconditionFnSig>(InKey);
	PreconditionFnKey = PreconditionFn ? InKey : "";
}

bool UTGoal::PreconditionCheck(const UTAgentContext& InContext) const
{
	if (PreconditionFn && *PreconditionFn)
	{
		return (*PreconditionFn)(InContext);
	}
	return true;
}
