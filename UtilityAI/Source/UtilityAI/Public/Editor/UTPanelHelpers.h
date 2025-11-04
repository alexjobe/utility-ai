#pragma once
#include <UTAction.h>
#include <UTConsideration.h>
#include <UTEffect.h>
#include <UTGoal.h>
#include <UTTrait.h>

using namespace UAI;

namespace UTEditor
{
	void RenderConsideration(const UTConsideration& Consideration);
	void RenderEffect(const UTEffect& Effect);
	void RenderEvaluationData(const UTEvaluationData& Data);
	void RenderAction(const UTAction& Action);
	void RenderGoal(const UTGoal& Goal);
	void RenderTrait(const UTTrait& Trait);
}