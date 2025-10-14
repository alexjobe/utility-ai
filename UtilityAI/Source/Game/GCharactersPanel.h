#pragma once
#include "GCharacter.h"
#include "UIEditorPanel.h"

using namespace UI;

namespace Game
{
	class GCharactersPanel : public UIEditorPanel
	{
	public:
		GCharactersPanel() : UIEditorPanel("Characters") {}
		void Render() override;
		void RenderCharacter(const GCharacter& Character);
	};
}