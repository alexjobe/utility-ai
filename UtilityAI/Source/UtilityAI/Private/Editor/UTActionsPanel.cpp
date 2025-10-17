#include "Editor/UTActionsPanel.h"
#include <Editor/UTPanelHelpers.h>
#include <imgui.h>
#include <UAI/UTObjectRegistry.h>

using namespace UTEditor;
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