#pragma once
#include "UTEditorPanel.h"
#include <memory>
#include <vector>

namespace UTEditor
{
class UTWindowManager
{
public:
	void AddPanel(std::unique_ptr<UTEditorPanel> InPanel);
	void RenderMenu();
	void RenderPanels();

private:
	std::vector<std::unique_ptr<UTEditorPanel>> Panels;
};
}