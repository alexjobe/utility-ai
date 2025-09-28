#include "EditorApp.h"

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <Logging/Logger.h>

EditorApp::EditorApp() = default;
EditorApp::~EditorApp() { Shutdown(); }

bool EditorApp::Init()
{
	if (SDL_Init(SDL_INIT_VIDEO) == false)
	{
		LOG_ERROR(std::format("Failed to initialize SDL: {}", SDL_GetError()))
		return false;
	}

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	Window_ = SDL_CreateWindow(
		"Utility AI Editor",
		1920, 1080,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN
	);

	if (!Window_)
	{
		LOG_ERROR(std::format("Failed to create window: {}", SDL_GetError()))
		return false;
	}

	GLContext_ = SDL_GL_CreateContext(Window_);
	if (!GLContext_)
	{
		LOG_ERROR(std::format("Failed to create GL context: {}", SDL_GetError()))
		return false;
	}
	SDL_GL_MakeCurrent(Window_, GLContext_);

	if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
	{
		LOG_ERROR("Failed to initialize GLAD")
		return false;
	}

	// ImGui setup
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& IO = ImGui::GetIO();

	IO.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	IO.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	IO.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

	ImGui::StyleColorsDark();

	// Adjust style for viewports (makes extra windows look like main window)
	ImGuiStyle& Style = ImGui::GetStyle();
	if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		Style.WindowRounding = 0.0f;
		Style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplSDL3_InitForOpenGL(Window_, GLContext_);
	ImGui_ImplOpenGL3_Init("#version 330");

	SDL_ShowWindow(Window_);

	bRunning_ = true;
	return true;
}

void EditorApp::Run()
{
	while (bRunning_) 
	{
		SDL_Event Event;
		while (SDL_PollEvent(&Event)) 
		{
			ImGui_ImplSDL3_ProcessEvent(&Event);
			if (Event.type == SDL_EVENT_QUIT)
			{
				bRunning_ = false;
			}
		}

		BeginFrame();
		RenderUI();
		EndFrame();
	}
}

void EditorApp::BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
}

void EditorApp::RenderUI()
{
	RenderDockSpace();
	ImGui::ShowDemoWindow();
}

void EditorApp::EndFrame()
{
	ImGuiIO& IO = ImGui::GetIO();

	ImGui::Render();
	glViewport(0, 0, (int)IO.DisplaySize.x, (int)IO.DisplaySize.y);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Handle multiple OS windows when viewports are enabled
	if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) 
	{
		SDL_Window* BackupWindow = SDL_GL_GetCurrentWindow();
		SDL_GLContext BackupContext = SDL_GL_GetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		SDL_GL_MakeCurrent(BackupWindow, BackupContext);
	}

	SDL_GL_SwapWindow(Window_);
}

void EditorApp::RenderDockSpace()
{
	static ImGuiDockNodeFlags DockspaceFlags = ImGuiDockNodeFlags_None;
	ImGuiWindowFlags WindowFlags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	ImGuiViewport* Viewport = ImGui::GetMainViewport();
	ImGui::SetNextWindowPos(Viewport->WorkPos);
	ImGui::SetNextWindowSize(Viewport->WorkSize);
	ImGui::SetNextWindowViewport(Viewport->ID);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	WindowFlags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse |
		ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
	WindowFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

	ImGui::Begin("DockSpace Demo", nullptr, WindowFlags);
	ImGui::PopStyleVar(2);

	ImGuiID DockspaceId = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(DockspaceId, ImVec2(0.0f, 0.0f), DockspaceFlags);
	ImGui::End();
}

void EditorApp::Shutdown()
{
	if (GLContext_ || Window_) 
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();
	}

	if (GLContext_) 
	{
		SDL_GL_DestroyContext(GLContext_);
		GLContext_ = nullptr;
	}
	if (Window_) 
	{
		SDL_DestroyWindow(Window_);
		Window_ = nullptr;
	}
	SDL_Quit();
}
