#include "GCharactersPanel.h"
#include "GWorld.h"
#include <imgui.h>

using namespace Game;

void GCharactersPanel::Render()
{
	if (ImGui::BeginChild("CharactersList", ImVec2(0, 0), true))
	{
		const auto& Characters = GWorld::Instance().GetAllCharacters();
		for (const auto& [_, Character] : Characters)
		{
			RenderCharacter(Character);
		}
		ImGui::EndChild();
	}
}

void GCharactersPanel::RenderCharacter(const GCharacter& Character)
{
	if (ImGui::TreeNode(Character.Key.c_str()))
	{
		if (ImGui::TreeNode("Traits"))
		{
			for (auto& Trait : Character.Traits)
			{
				ImGui::BulletText("%s", Trait.c_str());
			}
			ImGui::TreePop();
		}

		ImGui::Separator();
		if (ImGui::TreeNode("Needs"))
		{
			for (auto& [Need, Value] : Character.Needs)
			{
				ImGui::BulletText("%s: %f", ToString(Need).c_str(), Value);
			}
			ImGui::TreePop();
		}

		ImGui::Separator();
		if (ImGui::TreeNode("Stats"))
		{
			for (auto& [Stat, Value] : Character.CoreStats)
			{
				ImGui::BulletText("%s: %f", ToString(Stat).c_str(), Value);
			}
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}

