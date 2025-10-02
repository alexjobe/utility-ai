#pragma once
#include "UIEditorPanel.h"
#include <Core/UTAction.h>
#include <Core/UTConsideration.h>
#include <Core/UTEffect.h>

using namespace UI;

namespace UAI
{
class UTActionsPanel : public UIEditorPanel
{
public:
	UTActionsPanel() : UIEditorPanel("Actions") {}
	void Render() override;

private:
	void RenderAction(const UTAction& Action) const;
	void RenderConsideration(const UTConsideration& Consideration) const;
	void RenderEffect(const UTEffect& Effect) const;
	void RenderEvaluationData(const UTEvaluationData& Data) const;
};
}