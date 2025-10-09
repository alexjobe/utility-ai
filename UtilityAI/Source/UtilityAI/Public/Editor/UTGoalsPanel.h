#pragma once
#include "UIEditorPanel.h"

using namespace UI;

namespace UTEditor
{
	class UTGoalsPanel : public UIEditorPanel
	{
	public:
		UTGoalsPanel() : UIEditorPanel("Goals") {}
		void Render() override;
	};
}
