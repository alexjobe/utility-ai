#include "UTWindowManager.h"
#include <imgui.h>

using namespace UTEditor;

void UTWindowManager::Add(const char* Name, std::function<void()> RenderFn)
{
	Windows_.push_back({ Name, true, RenderFn });
}

void UTWindowManager::RenderMenu()
{
	if (ImGui::BeginMenu("Window"))
	{
		for (auto& Window : Windows_)
		{
			ImGui::MenuItem(Window.Name, nullptr, &Window.Open);
		}
		ImGui::EndMenu();
	}
}

void UTWindowManager::RenderWindows()
{
	for (auto& Window : Windows_)
	{
		if (Window.Open)
		{
			if (ImGui::Begin(Window.Name, &Window.Open))
			{
				Window.RenderFn();
			}
			ImGui::End();
		}
	}
}

