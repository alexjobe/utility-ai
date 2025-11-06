#pragma once
#include "Editor/UTEditorPanel.h"
#include "GCharacter.h"

using namespace UTEditor;

namespace Game
{
	class GCharactersPanel : public UTEditorPanel
	{
	public:
		GCharactersPanel();
		void Render() override;

	private:
		void RenderCharacter(GCharacter& Character);
		void RenderCharacterButtons(GCharacter& Character);
		void RenderChildWindows(GCharacter& Character);
		void RenderCharacterInfoWindow(GCharacter& Character);
		void RenderCurrentTraitsWindow(GCharacter& Character);
		void RenderCurrentGoalsWindow(GCharacter& Character);
		void RenderCurrentActionsWindow(GCharacter& Character);
	};
}