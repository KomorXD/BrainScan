#include "UIMenuBar.hpp"
#include "imgui/imgui.h"
#include "../Core.hpp"

UIMenuBar::UIMenuBar()
{
	ImVec2 windowSize = ImGui::GetMainViewport()->Size;

	m_Width  = windowSize.x;
	m_Height = 30.0f;

	ImVec2 pos = ImGui::GetMainViewport()->Pos;

	m_PosX = pos.x;
	m_PosY = pos.y;

	LOG_INFO("Initialized menu bar panel");
}

void UIMenuBar::Render()
{
	if(ImGui::BeginMainMenuBar())
	{
		Update();

		if(ImGui::BeginMenu("File"))
		{
			if(ImGui::MenuItem("Open", "Ctrl + O"))
			{
				LOG_INFO("File -> Open");
			}

			if(ImGui::MenuItem("Save", "Ctrl + S"))
			{
				LOG_INFO("File -> Save");
			}

			if(ImGui::MenuItem("Quit", "Ctrl + Q"))
			{
				LOG_INFO("File -> Quit");
			}

			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("Options"))
		{
			if(ImGui::MenuItem("Copy", "Ctrl + C"))
			{
				LOG_INFO("Options -> Copy");
			}

			if(ImGui::MenuItem("Cut", "Ctrl + X"))
			{
				LOG_INFO("Options -> Cut");
			}

			if(ImGui::MenuItem("Paste", "Ctrl + V"))
			{
				LOG_INFO("Options -> Paste");
			}
			
			ImGui::EndMenu();
		}

		if(ImGui::BeginMenu("Help"))
		{
			if(ImGui::MenuItem("nwm", "Ctrl + H"))
			{
				LOG_INFO("No help");
			}
			
			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}
}
