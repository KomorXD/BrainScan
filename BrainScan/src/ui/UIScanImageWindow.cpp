#include "UIScanImageWindow.hpp"
#include "imgui/imgui.h"
#include "../Core.hpp"
#include "../OpenGL.hpp"

#include <format>

UIScanImageWindow::UIScanImageWindow(int scanID, float posX, float posY, float height)
	: IUIPanel(posX, posY), m_ID(scanID)
{
	ImVec2 windowSize = ImGui::GetMainViewport()->Size;

	m_Width  = windowSize.x / 3.0f;
	m_Height = height;

	LOG_INFO("Initialized scan panel panel #{}", scanID);
}

void UIScanImageWindow::SetScanTexture(std::unique_ptr<Texture>&& text)
{
	m_ScanTexture = std::move(text);
}

void UIScanImageWindow::Render()
{
	ImGui::SetNextWindowSize(ImVec2(m_Width, m_Height));
	ImGui::SetNextWindowPos(ImVec2(m_PosX, m_PosY));

	ImGui::Begin(std::format("Scan image axis #{}", m_ID).c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	Update();

	ImGui::BeginChild(std::format("scan#{}", m_ID).c_str(), ImVec2(m_Width - 16.0f, m_Height - ImGui::GetFontSize() * 1.7f - 8.0f), true);

	if (ImGui::IsWindowHovered())
	{
		LOG_WARN("Scan #{} hovered on.", m_ID);
	}

	if(m_ScanTexture)
	{
		ImGui::Image((void*)(intptr_t)m_ScanTexture->GetID(), ImVec2(m_Width - 32.0f, m_Height - ImGui::GetFontSize() * 1.7f - 24.0f));
	}
	
	ImGui::EndChild();

	ImGui::End();
}
