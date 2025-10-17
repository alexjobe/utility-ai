#pragma once
#include "UTEditorPanel.h"

namespace UTEditor
{
class UTGoalsPanel : public UTEditorPanel
{
public:
	UTGoalsPanel() : UTEditorPanel("Goals") {}
	void Render() override;
};
}
