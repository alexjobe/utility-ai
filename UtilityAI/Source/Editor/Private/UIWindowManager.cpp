#include "UIWindowManager.h"
#include <imgui.h>

using namespace UI;

void UIWindowManager::AddPanel(std::unique_ptr<UIEditorPanel> InPanel)
{
	Panels.push_back(std::move(InPanel));
}


void UIWindowManager::RenderMenu()
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

void UIWindowManager::RenderPanels()
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

