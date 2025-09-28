#pragma once
#include <SDL3/SDL.h>

class EditorApp {
public:
	EditorApp();
	~EditorApp();

	bool Init();
	void Run();
	void Shutdown();

private:
	void BeginFrame();
	void EndFrame();
	void RenderUI();
	void RenderDockSpace();

	SDL_Window* Window_ = nullptr;
	SDL_GLContext GLContext_ = nullptr;
	bool bRunning_ = false;
};