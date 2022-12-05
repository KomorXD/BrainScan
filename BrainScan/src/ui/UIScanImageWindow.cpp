#include "UIScanImageWindow.hpp"
#include "imgui/imgui.h"
#include "../Core.hpp"

#include <format>

UIScanImageWindow::UIScanImageWindow(int scanID, float posX, float posY, float height)
	: IUIPanel(posX, posY), m_ID(scanID)
{
	ImVec2 windowSize = ImGui::GetMainViewport()->Size;

	m_Width  = windowSize.x / 3.0f;
	m_Height = height;

	LOG_INFO("Initialized scan panel panel #{}", scanID);
}

void UIScanImageWindow::Render()
{
	ImVec2 windowPos = ImGui::GetMainViewport()->Pos;

	ImGui::SetNextWindowSize(ImVec2(m_Width, m_Height), ImGuiCond_FirstUseEver);
	ImGui::SetNextWindowPos(ImVec2(m_PosX, m_PosY), ImGuiCond_FirstUseEver);

	ImGui::Begin(std::format("Scan image axis #{}", m_ID).c_str(), nullptr, ImGuiWindowFlags_NoCollapse);

	Update();

	ImGui::BeginChild(std::format("scan#{}", m_ID).c_str(), ImVec2(m_Width - 16.0f, m_Height - ImGui::GetFontSize() * 1.7f - 8.0f), true);
	ImGui::Text("Area for a scan");
	ImGui::EndChild();

	ImGui::End();
}
