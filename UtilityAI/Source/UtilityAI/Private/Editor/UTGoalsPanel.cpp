#include "Editor/UTGoalsPanel.h"
#include <Core/UTObjectRegistry.h>
#include <Editor/UTPanelHelpers.h>
#include <imgui.h>
#include <UAI/UTGoal.h>

using namespace UTEditor;
using namespace UTCore;
using namespace UAI;

void UTGoalsPanel::Render()
{
	if (ImGui::BeginChild("GoalsList", ImVec2(0, 0), true))
	{
		const auto Goals = UTObjectRegistry<UTGoal>::Instance().GetAll();
		for (const auto* Goal : Goals)
		{
			if (Goal)
			{
				RenderGoal(*Goal);
			}
		}
		ImGui::EndChild();
	}
}
