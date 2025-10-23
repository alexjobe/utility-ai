#include "Editor/UTActionsPanel.h"
#include <Core/UTObjectRegistry.h>
#include <Editor/UTPanelHelpers.h>
#include <imgui.h>

using namespace UTEditor;
using namespace UTCore;
using namespace UAI;

void UTActionsPanel::Render()
{
	if (ImGui::BeginChild("ActionsList", ImVec2(0, 0), true))
	{
		const auto& Actions = UTObjectRegistry<UTAction>::Instance().GetAll();
		for (const auto& [_, Action] : Actions)
		{
			RenderAction(Action);
		}
		ImGui::EndChild();
	}
}