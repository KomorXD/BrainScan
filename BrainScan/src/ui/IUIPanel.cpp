#include "IUIPanel.hpp"
#include "imgui/imgui.h"

IUIPanel::IUIPanel(float posX, float posY)
	: m_PosX(posX), m_PosY(posY)
{}

void IUIPanel::Update()
{
	ImVec2 size = ImGui::GetWindowSize();

	m_Width  = size.x;
	m_Height = size.y;

	ImVec2 pos = ImGui::GetWindowPos();

	m_PosX = pos.x;
	m_PosY = pos.y;
}
