#include "UIEditorApp.h"

#include <backends/imgui_impl_opengl3.h>
#include <backends/imgui_impl_sdl3.h>
#include <glad/glad.h>
#include <imgui.h>
#include <Logging/Logger.h>

using namespace UI;

UIEditorApp::UIEditorApp()
	: Window(nullptr)
	, GLContext(nullptr)
	, bRunning(false)
	, TargetFPS(60)
	, TargetFrameTime(1.f / TargetFPS)
	, FontScale(2.f)
{
}

UIEditorApp::~UIEditorApp() 
{ 
	Shutdown(); 
}

bool UIEditorApp::Init()
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

	Window = SDL_CreateWindow(
		"Utility AI Editor",
		1920, 1080,
		SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_HIDDEN
	);

	if (!Window)
	{
		LOG_ERROR(std::format("Failed to create window: {}", SDL_GetError()))
		return false;
	}

	GLContext = SDL_GL_CreateContext(Window);
	if (!GLContext)
	{
		LOG_ERROR(std::format("Failed to create GL context: {}", SDL_GetError()))
		return false;
	}
	SDL_GL_MakeCurrent(Window, GLContext);

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

	IO.FontGlobalScale = FontScale;

	ImGui::StyleColorsDark();

	// Adjust style for viewports (makes extra windows look like main window)
	ImGuiStyle& Style = ImGui::GetStyle();
	if (IO.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		Style.WindowRounding = 0.0f;
		Style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplSDL3_InitForOpenGL(Window, GLContext);
	ImGui_ImplOpenGL3_Init("#version 330");

	SDL_ShowWindow(Window);

	bRunning = true;
	return true;
}

void UIEditorApp::Run()
{
	const uint64_t PerfFreq = SDL_GetPerformanceFrequency();
	uint64_t LastCounter = SDL_GetPerformanceCounter();

	while (bRunning) 
	{
		SDL_Event Event;
		while (SDL_PollEvent(&Event)) 
		{
			HandleEvent(Event);
		}

		// Timing
		const uint64_t CurrentCounter = SDL_GetPerformanceCounter();
		const double DeltaTime = static_cast<double>(CurrentCounter - LastCounter) / static_cast<double>(PerfFreq);
		LastCounter = CurrentCounter;

		// Update/Render
		BeginFrame();
		RenderUI();
		EndFrame();

		// Frame Cap
		const uint64_t FrameEndCounter = SDL_GetPerformanceCounter();
		const double FrameDuration = static_cast<double>(FrameEndCounter - CurrentCounter) / static_cast<double>(PerfFreq);

		if (FrameDuration < TargetFrameTime)
		{
			const double SleepTimeSec = TargetFrameTime - FrameDuration;
			SDL_Delay((Uint32)(SleepTimeSec * 1000.0));
		}
	}
}

bool UIEditorApp::HandleEvent(const SDL_Event& Event)
{
	// Always let ImGui process events first
	ImGui_ImplSDL3_ProcessEvent(&Event);

	switch (Event.type)
	{
	case SDL_EVENT_QUIT:
		bRunning = false;
		return true;

	case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
		// Quit only if it's the main SDL window
		if (Event.window.windowID == SDL_GetWindowID(Window))
		{
			bRunning = false;
			return true;
		}
		break;

	default:
		break;
	}

	return false; // not consumed
}

void UIEditorApp::BeginFrame()
{
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL3_NewFrame();
	ImGui::NewFrame();
}

void UIEditorApp::RenderUI()
{
	RenderDockSpace();
	WindowManager.RenderPanels();
}

void UIEditorApp::EndFrame()
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

	SDL_GL_SwapWindow(Window);
}

void UIEditorApp::RenderDockSpace()
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

	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Exit", "Alt+F4")) // show hotkey hint
			{
				bRunning = false;
			}
			ImGui::EndMenu();
		}
		WindowManager.RenderMenu();
		ImGui::EndMenuBar();
	}

	ImGuiID DockspaceId = ImGui::GetID("MyDockSpace");
	ImGui::DockSpace(DockspaceId, ImVec2(0.0f, 0.0f), DockspaceFlags);
	ImGui::End();
}

void UIEditorApp::Shutdown()
{
	if (GLContext || Window) 
	{
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplSDL3_Shutdown();
		ImGui::DestroyContext();
	}

	if (GLContext) 
	{
		SDL_GL_DestroyContext(GLContext);
		GLContext = nullptr;
	}
	if (Window) 
	{
		SDL_DestroyWindow(Window);
		Window = nullptr;
	}
	SDL_Quit();
}
