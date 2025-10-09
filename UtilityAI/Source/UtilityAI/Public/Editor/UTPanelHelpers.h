#pragma once
#include <Core/UTAction.h>
#include <Core/UTConsideration.h>
#include <Core/UTEffect.h>
#include <Core/UTGoal.h>
#include <Core/UTTrait.h>

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