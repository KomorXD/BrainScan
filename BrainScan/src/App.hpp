#pragma once

#include <string>
#include <memory>

#include "scenes/IScene.hpp"

struct GLFWwindow;

class App
{
	public:
		~App() = default;

		void Run();
		void SetNextScene(std::unique_ptr<IScene>&& newScene);
		void SetWindowShouldClose(bool status);

		inline GLFWwindow* GetWindow() { return m_Window; }

		static App& GetInstance();

	private:
		App(int32_t windowWidth, int32_t windowHeight, const std::string& title);
		
		GLFWwindow* m_Window = nullptr;
		std::unique_ptr<IScene> m_CurrentScene;
		std::unique_ptr<IScene> m_NextScene = nullptr;
		static inline App* s_Instance;
};