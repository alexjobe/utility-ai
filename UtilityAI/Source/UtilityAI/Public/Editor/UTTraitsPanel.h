#pragma once
#include "UIEditorPanel.h"

using namespace UI;

namespace UTEditor
{
	class UTTraitsPanel : public UIEditorPanel
	{
	public:
		UTTraitsPanel() : UIEditorPanel("Traits") {}
		void Render() override;
	};
}