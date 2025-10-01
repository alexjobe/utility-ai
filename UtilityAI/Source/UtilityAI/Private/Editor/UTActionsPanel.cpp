#include "Editor/UTActionsPanel.h"
#include <Core/UTActionRegistry.h>
#include <imgui.h>

using namespace UAI;

void UTActionsPanel::Render()
{
	if (ImGui::BeginChild("ActionsList", ImVec2(0, 0), true))
	{
		const auto& Actions = UTActionRegistry::Instance().GetActions();
		for (const auto& [_, Action] : Actions)
		{
			if (ImGui::TreeNode(Action.GetKey().c_str()))
			{
				ImGui::Text("Tags:");
				for (auto& Tag : Action.Tags)
				{
					ImGui::BulletText("%s", Tag.c_str());
				}

				ImGui::Separator();
				if (ImGui::TreeNode("Effects"))
				{
					for (auto& [_, Effect] : Action.GetEffects())
					{
						ImGui::BulletText("%s", Effect.Key.c_str());
					}
					ImGui::TreePop();
				}

				ImGui::TreePop();
			}
		}
		ImGui::EndChild();
	}
}

