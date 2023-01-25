#include "App.hpp"
#include "Logger.hpp"
#include "OpenGL.hpp"
#include "scenes/EmptyScene.hpp"
#include "scenes/AdvancedScene.hpp"

#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_glfw.h>

#include <GLFW/glfw3.h>

#include <iostream>
#include <format>

App::App(int32_t windowWidth, int32_t windowHeight, const std::string& title)
{
	if(!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW.\n";

		return;
	}
	
	glfwSetErrorCallback([](int32_t error, const char* description)
	{
		LOG_WARN("GLFW has thrown an error #{}: {}", error, description);
	});
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	m_Window = glfwCreateWindow(windowWidth, windowHeight, title.c_str(), nullptr, nullptr);

	if(!m_Window)
	{
		LOG_CRITICAL("Failed to create a window.");

		glfwTerminate();

		return;
	}

	glfwMakeContextCurrent(m_Window);
	glfwSwapInterval(1);
	
	if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		LOG_CRITICAL("Failed to load GLAD.");

		glfwTerminate();

		return;
	}

	s_Instance = this;
}

void App::Run()
{
	if(!s_Instance)
	{
		LOG_CRITICAL("Something went wrong in the app's initialization. The program will now terminate.");

		return;
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();

	io.Fonts->AddFontDefault();

	ImFont* font = io.Fonts->AddFontFromFileTTF("res/fonts/OpenSans.ttf", 24.0f);

	IM_ASSERT(font != nullptr);

	io.ConfigWindowsMoveFromTitleBarOnly = true;

	ImGui::StyleColorsDark();

	ImGuiStyle& style = ImGui::GetStyle();

	ImGui_ImplGlfw_InitForOpenGL(m_Window, true);
	ImGui_ImplOpenGL3_Init();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	m_CurrentScene = std::make_unique<EmptyScene>();
	
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	while(!glfwWindowShouldClose(m_Window))
	{
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		
		if (m_NextScene)
		{
			m_CurrentScene = std::move(m_NextScene);
		}

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

		glfwPollEvents();
		glfwSwapBuffers(m_Window);
	}

	glfwDestroyWindow(m_Window);
	glfwTerminate();
}

void App::SetNextScene(std::unique_ptr<IScene>&& newScene)
{
	m_NextScene = std::move(newScene);
}

void App::SetWindowShouldClose(bool status)
{
	glfwSetWindowShouldClose(m_Window, status);
}

App& App::GetInstance()
{
	static App app(1280, 720, "BRAINSCAN");

	return app;
}
