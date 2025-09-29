#pragma once
#include <SDL3/SDL.h>
#include <UTWindowManager.h>

namespace UTEditor
{
class UTEditorApp {
public:
	UTEditorApp();
	~UTEditorApp();

	bool Init();
	void Run();
	void Shutdown();

private:
	bool HandleEvent(const SDL_Event& Event);
	void BeginFrame();
	void EndFrame();
	void RenderUI();
	void RenderDockSpace();

	SDL_Window* Window_ = nullptr;
	SDL_GLContext GLContext_ = nullptr;
	bool bRunning_ = false;
	float FontScale_ = 2.0f;

	UTWindowManager WindowManager;
};
}