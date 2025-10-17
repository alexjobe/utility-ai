#pragma once
#include <UAI/UTAction.h>
#include <UAI/UTConsideration.h>
#include <UAI/UTEffect.h>
#include <UAI/UTGoal.h>
#include <UAI/UTTrait.h>

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