#include "GCharactersPanel.h"
#include "GWorld.h"
#include <Editor/UTPanelHelpers.h>
#include <format>
#include <imgui.h>

using namespace Game;

GCharactersPanel::GCharactersPanel()
	: UTEditorPanel("Characters")
{
}

void GCharactersPanel::Render()
{
	if (ImGui::Button("Add Character"))
	{
		static int NewID = 0;
		std::string Name = std::format("Character_{}", NewID++);
		GWorld::Instance().AddCharacter(GCharacter{ Name });
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
	if (ImGui::TreeNode(std::format("{} - {}##{}", Character.Name, Character.Profession, Character.GetKey()).c_str()))
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

		RenderCharacterButtons(Character);
		RenderChildWindows(Character);

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

		char NewTrait[64] = "";
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

void GCharactersPanel::RenderCharacterButtons(GCharacter& Character)
{
	ImGui::Separator();
	if (ImGui::Button(("Change Info##" + Character.GetKey()).c_str()))
	{
		Character.RenderComp.bShowInfoWindow = !Character.RenderComp.bShowInfoWindow;
	}

	ImGui::Separator();
	if (ImGui::Button(("Goals##" + Character.GetKey()).c_str()))
	{
		Character.RenderComp.bShowGoalsWindow = !Character.RenderComp.bShowGoalsWindow;
	}

	ImGui::SameLine();
	if (ImGui::Button(("Regenerate Goals##" + Character.GetKey()).c_str()))
	{
		Character.UpdateGoals();
	}
}


void GCharactersPanel::RenderChildWindows(GCharacter& Character)
{
	if (Character.RenderComp.bShowInfoWindow)
	{
		ImGui::SetNextWindowSize(ImVec2(800, 200), ImGuiCond_FirstUseEver);
		if (ImGui::Begin(
			(std::format("Change Info - {}##{}", Character.Name, Character.GetKey())).c_str(),
			&Character.RenderComp.bShowInfoWindow,
			ImGuiWindowFlags_NoDocking)) // Prevents going into dockspace
		{
			RenderCharacterInfoWindow(Character);
		}
		ImGui::End();
	}

	if (Character.RenderComp.bShowGoalsWindow)
	{
		ImGui::SetNextWindowSize(ImVec2(400, 200), ImGuiCond_FirstUseEver);
		if (ImGui::Begin(
			(std::format("Goals - {}##{}", Character.Name, Character.GetKey())).c_str(),
			&Character.RenderComp.bShowGoalsWindow,
			ImGuiWindowFlags_NoDocking)) // Prevents going into dockspace
		{
			RenderCurrentGoalsWindow(Character);
		}
		ImGui::End();
	}
}

void GCharactersPanel::RenderCharacterInfoWindow(GCharacter& Character)
{
	if (ImGui::BeginChild("CharacterInfo", ImVec2(0, 0), true))
	{
		// Name edit
		char NewName[128] = "";
		if (ImGui::InputText("Change Name", NewName, sizeof(NewName), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (strlen(NewName) > 0)
			{
				Character.Name = NewName;
				NewName[0] = '\0';
			}
		}

		ImGui::Separator();
		// Profession edit
		char NewProfession[128] = "";
		if (ImGui::InputText("Change Profession", NewProfession, sizeof(NewProfession), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (strlen(NewProfession) > 0)
			{
				Character.Profession = NewProfession;
				NewProfession[0] = '\0';
			}
		}
		ImGui::EndChild();
	}
}

void GCharactersPanel::RenderCurrentGoalsWindow(GCharacter& Character)
{
	if (ImGui::BeginChild("GoalsList", ImVec2(0, 0), true))
	{
		for (const auto& Goal : Character.GetCurrentGoals())
		{
			UTEditor::RenderGoal(Goal);
		}
		ImGui::EndChild();
	}
}
