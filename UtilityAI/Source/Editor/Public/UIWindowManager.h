#pragma once
#include "UIEditorPanel.h"
#include <memory>
#include <vector>

namespace UI
{
class UIWindowManager
{
public:
	void AddPanel(std::unique_ptr<UIEditorPanel> InPanel);
	void RenderMenu();
	void RenderPanels();

private:
	std::vector<std::unique_ptr<UIEditorPanel>> Panels;
};
}