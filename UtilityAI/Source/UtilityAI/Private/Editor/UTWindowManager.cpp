#include "Editor/UTWindowManager.h"
#include <imgui.h>

using namespace UTEditor;

void UTWindowManager::AddPanel(std::unique_ptr<UTEditorPanel> InPanel)
{
	Panels.push_back(std::move(InPanel));
}

void UTWindowManager::RenderMenu()
{
	if (ImGui::BeginMenu("Window"))
	{
		for (auto& Panel : Panels)
		{
			ImGui::MenuItem(Panel->GetName().c_str(), nullptr, Panel->GetOpenPtr());
		}
		ImGui::EndMenu();
	}
}

void UTWindowManager::RenderPanels()
{
	for (auto& Panel : Panels)
	{
		if (*Panel->GetOpenPtr())
		{
			if (ImGui::Begin(Panel->GetName().c_str(), Panel->GetOpenPtr()))
			{
				Panel->Render();
			}
			ImGui::End();
		}
	}
}

