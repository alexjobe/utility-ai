#include "Editor/UTActionsPanel.h"
#include <Core/UTActionRegistry.h>
#include <Editor/UTPanelHelpers.h>
#include <imgui.h>

using namespace UTEditor;
using namespace UAI;

void UTActionsPanel::Render()
{
	if (ImGui::BeginChild("ActionsList", ImVec2(0, 0), true))
	{
		const auto& Actions = UTActionRegistry::Instance().GetActions();
		for (const auto& [_, Action] : Actions)
		{
			RenderAction(Action);
		}
		ImGui::EndChild();
	}
}