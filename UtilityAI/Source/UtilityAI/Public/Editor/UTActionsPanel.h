#pragma once
#include "UIEditorPanel.h"

using namespace UI;

namespace UAI
{
class UTActionsPanel : public UIEditorPanel
{
public:
	UTActionsPanel() : UIEditorPanel("Actions") {}
	void Render() override;
};
}