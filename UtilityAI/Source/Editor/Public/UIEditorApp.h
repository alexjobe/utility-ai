#pragma once
#include <SDL3/SDL.h>
#include <UIWindowManager.h>

namespace UI
{
class UIEditorApp {
public:
	UIEditorApp();
	~UIEditorApp();

	bool Init();
	void Run();
	void Shutdown();

	UIWindowManager& GetWindowManager() { return WindowManager; }

private:
	bool HandleEvent(const SDL_Event& Event);
	void BeginFrame();
	void EndFrame();
	void RenderUI();
	void RenderDockSpace();

	SDL_Window* Window = nullptr;
	SDL_GLContext GLContext = nullptr;
	bool bRunning = false;
	float FontScale = 2.0f;

	UIWindowManager WindowManager;
};
}