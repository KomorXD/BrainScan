#include "UIToolSettings.hpp"
#include "imgui/imgui.h"
#include "../Core.hpp"

UIToolSettings::UIToolSettings(float posX, float posY)
	: IUIPanel(posX, posY)
{
	ImVec2 windowSize = ImGui::GetMainViewport()->Size;
	ImVec2 windowPos  = ImGui::GetMainViewport()->Pos;

	m_Width  = windowSize.x / 3.0f;
	m_Height = windowSize.y - posY + windowPos.y;

	LOG_INFO("Initialized tool settings panel");
}

void UIToolSettings::Render()
{
	ImGui::SetNextWindowSize(ImVec2(m_Width, m_Height), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(m_PosX, m_PosY), ImGuiCond_FirstUseEver);

	ImGui::Begin("Tool settings", nullptr, ImGuiWindowFlags_NoCollapse);
	
	Update();

	ImGui::Text("xd");

	ImGui::End();
}
