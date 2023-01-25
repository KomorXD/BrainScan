#include "UIMenuBar.hpp"
#include "imgui/imgui.h"
#include "imgui/ImGuiFileDialog.h"
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

void UIMenuBar::PushMenu(const std::string& label)
{
	m_Menus.emplace_back(label);
}

void UIMenuBar::PushMenuItem(const std::string& label, const std::string& shortcut, const std::function<void(void)>& f)
{
	if (m_Menus.empty())
	{
		return;
	}

	MenuItem mt{ label, shortcut, f };

	m_Menus.back().items.push_back(std::move(mt));
}

void UIMenuBar::Render()
{
	if(ImGui::BeginMainMenuBar())
	{
		Update();

		for (const auto& menu : m_Menus)
		{
			if (ImGui::BeginMenu(menu.label.c_str()))
			{
				for (const auto& item : menu.items)
				{
					if (ImGui::MenuItem(item.label.c_str(), item.shortcut.c_str()))
					{
						item.func();
					}
				}

				ImGui::EndMenu();
			}
		}

		ImGui::EndMainMenuBar();
	}
}
