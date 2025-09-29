#pragma once
#include <functional>

namespace UTEditor
{
struct UTEditorWindow
{
	const char* Name;
	bool Open = true;
	std::function<void()> RenderFn;
};

class UTWindowManager
{
public:
	void Add(const char* Name, std::function<void()> RenderFn);

	void RenderMenu();

	void RenderWindows();

private:
	std::vector<UTEditorWindow> Windows_;
};
}