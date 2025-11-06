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

void GCharactersPanel::RenderCharacterButtons(GCharacter& Character)
{
	ImGui::Separator();
	if (ImGui::Button(("Change Info##" + Character.GetKey()).c_str()))
	{
		Character.RenderComp.bShowInfoWindow = !Character.RenderComp.bShowInfoWindow;
	}

	ImGui::Separator();
	if (ImGui::Button(("Traits##" + Character.GetKey()).c_str()))
	{
		Character.RenderComp.bShowTraitsWindow = !Character.RenderComp.bShowTraitsWindow;
	}

	ImGui::Separator();
	if (ImGui::Button(("Goals##" + Character.GetKey()).c_str()))
	{
		Character.RenderComp.bShowGoalsWindow = !Character.RenderComp.bShowGoalsWindow;
	}

	ImGui::SameLine();
	if (ImGui::Button(("Generate Goals##" + Character.GetKey()).c_str()))
	{
		Character.UpdateGoals();
	}

	ImGui::Separator();
	if (ImGui::Button(("Actions##" + Character.GetKey()).c_str()))
	{
		Character.RenderComp.bShowActionsWindow = !Character.RenderComp.bShowActionsWindow;
	}

	ImGui::SameLine();
	if (ImGui::Button(("Generate Actions##" + Character.GetKey()).c_str()))
	{
		Character.UpdateActions();
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

	if (Character.RenderComp.bShowTraitsWindow)
	{
		ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_FirstUseEver);
		if (ImGui::Begin(
			(std::format("Traits - {}##{}", Character.Name, Character.GetKey())).c_str(),
			&Character.RenderComp.bShowTraitsWindow,
			ImGuiWindowFlags_NoDocking))
		{
			RenderCurrentTraitsWindow(Character);
		}
		ImGui::End();
	}

	if (Character.RenderComp.bShowGoalsWindow)
	{
		ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_FirstUseEver);
		if (ImGui::Begin(
			(std::format("Goals - {}##{}", Character.Name, Character.GetKey())).c_str(),
			&Character.RenderComp.bShowGoalsWindow,
			ImGuiWindowFlags_NoDocking))
		{
			RenderCurrentGoalsWindow(Character);
		}
		ImGui::End();
	}

	if (Character.RenderComp.bShowActionsWindow)
	{
		ImGui::SetNextWindowSize(ImVec2(500, 600), ImGuiCond_FirstUseEver);
		if (ImGui::Begin(
			(std::format("Actions - {}##{}", Character.Name, Character.GetKey())).c_str(),
			&Character.RenderComp.bShowActionsWindow,
			ImGuiWindowFlags_NoDocking))
		{
			RenderCurrentActionsWindow(Character);
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

void GCharactersPanel::RenderCurrentTraitsWindow(GCharacter& Character)
{
	if (ImGui::BeginChild("TraitsList", ImVec2(0, 0), true))
	{
		char NewTrait[64] = "";
		if (ImGui::InputText("Add Trait", NewTrait, sizeof(NewTrait), ImGuiInputTextFlags_EnterReturnsTrue))
		{
			if (strlen(NewTrait) > 0)
			{
				Character.AddTrait(NewTrait);
				NewTrait[0] = '\0';
			}
		}

		ImGui::Separator();
		for (const auto& [_, Trait] : Character.GetCurrentTraits())
		{
			if (Trait)
			{
				UTEditor::RenderTrait(*Trait);
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

void GCharactersPanel::RenderCurrentActionsWindow(GCharacter& Character)
{
	if (ImGui::BeginChild("ActionsList", ImVec2(0, 0), true))
	{
		for (const auto& Action : Character.GetCurrentActions())
		{
			UTEditor::RenderAction(Action);
		}
		ImGui::EndChild();
	}
}
