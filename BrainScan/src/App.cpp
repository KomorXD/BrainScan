#include "App.hpp"
#include "OpenGL.hpp"
#include "scenes/EmptyScene.hpp"

#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>
#include <imgui/ImGuiFileDialog.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <format>

static void glfw_error_callback(int error, const char* description)
{
	std::cerr << std::format("GLFW error #{} - {}", error, description) << "\n";
}

App::App(int32_t windowWidth, int32_t windowHeight, const std::string& title)
{
	if(!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW.\n";

		return;
	}
	
	glfwSetErrorCallback(glfw_error_callback);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	m_Window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), nullptr, nullptr);

	if(!m_Window)
	{
		std::cerr << "Failed to create a window.\n";
		glfwTerminate();

		return;
	}

	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(1);
	
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cerr << "Failed to load GLAD.\n";

		glfwTerminate();

		return;
	}

	s_Instance = this;
}

void App::Run()
{
	if(!s_Instance)
	{
		std::cerr << "Something went wrong in the app's initialization. The program will now terminate.\n";

		return;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.Fonts->AddFontDefault();

	ImFont* font = io.Fonts->AddFontFromFileTTF("res/fonts/OpenSans.ttf", 24.0f);

	IM_ASSERT(font != nullptr);

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	io.ConfigWindowsMoveFromTitleBarOnly = true;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();

	if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		style.WindowRounding = 0.0f;
		style.Colors[ImGuiCol_WindowBg].w = 1.0f;
	}

	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	ImGui_ImplOpenGL3_Init();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	m_CurrentScene = std::make_unique<EmptyScene>();
	
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();

		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

	while(!glfwWindowShouldClose(m_Window))
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();

		ImGui::NewFrame();
		ImGui::PushFont(font);

		m_CurrentScene->Input();
		m_CurrentScene->Update();
		m_CurrentScene->Render();

		ImGui::PopFont();
		ImGui::Render();

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if(io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();

			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}

		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

App& App::GetInstance()
{
	static App app(1280, 720, "BRAINSCAN");

	return app;
}
