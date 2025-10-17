#pragma once
#include <Editor/UTWindowManager.h>
#include <SDL3/SDL.h>

namespace UTEditor
{
class UTEditorApp {
public:
	UTEditorApp();
	~UTEditorApp();

	bool Init();
	void Run();
	void Shutdown();

	UTWindowManager& GetWindowManager() { return WindowManager; }

private:
	bool HandleEvent(const SDL_Event& Event);
	void BeginFrame();
	void EndFrame();
	void RenderUI();
	void RenderDockSpace();

	SDL_Window* Window;
	SDL_GLContext GLContext;
	bool bRunning;
	int TargetFPS;
	double TargetFrameTime;
	float FontScale;

	UTWindowManager WindowManager;
};
}