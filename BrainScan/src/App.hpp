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

		static App& GetInstance();

	private:
		App(int32_t windowWidth, int32_t windowHeight, const std::string& title);
		
		GLFWwindow* m_Window = nullptr;
		std::unique_ptr<IScene> m_CurrentScene;

		static inline App* s_Instance;
};