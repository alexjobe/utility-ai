#pragma once
#include <string>

namespace UTEditor
{
class UTEditorPanel
{
public:
	UTEditorPanel(const std::string& InName) : Name(InName), bOpen(true) {}
	virtual ~UTEditorPanel() = default;

	const std::string& GetName() const { return Name; }
	bool* GetOpenPtr() { return &bOpen; }

	virtual void Render() = 0; // Each panel defines its own UI

protected:
	std::string Name;
	bool bOpen;
};
}