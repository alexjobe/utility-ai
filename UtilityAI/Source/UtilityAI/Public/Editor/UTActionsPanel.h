#pragma once
#include "UTEditorPanel.h"

namespace UTEditor
{
class UTActionsPanel : public UTEditorPanel
{
public:
	UTActionsPanel() : UTEditorPanel("Actions") {}
	void Render() override;
};
}