#include "Editor/UTTraitsPanel.h"
#include <Core/UTTraitRegistry.h>
#include <Editor/UTPanelHelpers.h>
#include <imgui.h>

using namespace UTEditor;
using namespace UAI;

void UTTraitsPanel::Render()
{
	if (ImGui::BeginChild("TraitsList", ImVec2(0, 0), true))
	{
		const auto& Traits = UTTraitRegistry::Instance().GetTraits();
		for (const auto& [_, Trait] : Traits)
		{
			RenderTrait(Trait);
		}
		ImGui::EndChild();
	}
}

