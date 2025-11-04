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
		void RenderTraits(GCharacter& Character);
		void RenderCurrentGoals(GCharacter& Character);
	};
}