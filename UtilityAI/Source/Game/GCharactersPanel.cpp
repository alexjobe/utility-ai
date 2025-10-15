#include "GCharactersPanel.h"
#include "GWorld.h"
#include <format>
#include <imgui.h>

using namespace Game;

void GCharactersPanel::Render()
{
	if (ImGui::Button("Add Character"))
	{
		static int NewID = 0;
		std::string Key = std::format("Character_{}", NewID++);
		GWorld::Instance().AddCharacter(GCharacter{ Key });
	}

	ImGui::Separator();

	if (ImGui::BeginChild("CharactersList", ImVec2(0, 0), true))
	{
		auto& Characters = GWorld::Instance().GetAllCharacters();
		for (auto& [_, Character] : Characters)
		{
			RenderCharacter(Character);
		}
		ImGui::EndChild();
	}
}

void GCharactersPanel::RenderCharacter(GCharacter& Character)
{
	if (ImGui::TreeNode(Character.Key.c_str()))
	{
		RenderTraits(Character);

		ImGui::Separator();
		if (ImGui::TreeNode("Needs"))
		{
			for (auto& [Need, Value] : Character.Needs)
			{
				float NewVal = Value;
				if (ImGui::DragFloat(ToString(Need).c_str(), &NewVal, 1.f, MIN_NEED, MAX_NEED))
				{
					Value = NewVal;
				}
			}
			ImGui::TreePop();
		}

		ImGui::Separator();
		if (ImGui::TreeNode("Stats"))
		{
			for (auto& [Stat, Value] : Character.CoreStats)
			{
				float NewVal = Value;
				if (ImGui::DragFloat(ToString(Stat).c_str(), &NewVal, 1.f, MIN_CORE_STAT, MAX_CORE_STAT))
				{
					Value = NewVal;
				}
			}
			ImGui::TreePop();
		}

		ImGui::TreePop();
	}
}

void GCharactersPanel::RenderTraits(GCharacter& Character)
{
	if (ImGui::TreeNode("Traits"))
	{
		for (auto It = Character.Traits.begin(); It != Character.Traits.end(); )
		{
			ImGui::BulletText("%s", It->c_str());
			ImGui::SameLine();
			if (ImGui::SmallButton(std::format("Remove##{}", *It).c_str()))
			{
				It = Character.Traits.erase(It);
			}
			else
			{
				++It;
			}
		}

		static char NewTrait[64] = "";
		if (ImGui::InputText("New Trait", NewTrait, sizeof(NewTrait), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (strlen(NewTrait) > 0)
			{
				Character.Traits.insert(NewTrait);
				NewTrait[0] = '\0';
			}
		}

		ImGui::TreePop();
	}
}

