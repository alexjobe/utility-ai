#include "Editor/UTGoalsPanel.h"
#include <Editor/UTPanelHelpers.h>
#include <imgui.h>
#include <UAI/UTGoal.h>
#include <UAI/UTObjectRegistry.h>

using namespace UTEditor;
using namespace UAI;

void UTGoalsPanel::Render()
{
	if (ImGui::BeginChild("GoalsList", ImVec2(0, 0), true))
	{
		const auto& Goals = UTObjectRegistry<UTGoal>::Instance().GetAll();
		for (const auto& [_, Goal] : Goals)
		{
			RenderGoal(Goal);
		}
		ImGui::EndChild();
	}
}
