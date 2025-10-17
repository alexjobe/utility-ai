#pragma once
#include "UTEditorPanel.h"

namespace UTEditor
{
	class UTTraitsPanel : public UTEditorPanel
	{
	public:
		UTTraitsPanel() : UTEditorPanel("Traits") {}
		void Render() override;
	};
}