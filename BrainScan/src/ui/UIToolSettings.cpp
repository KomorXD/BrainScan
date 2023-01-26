#include "UIToolSettings.hpp"
#include "imgui/imgui.h"
#include "../Core.hpp"
#include "../scenes/IScene.hpp"

UIToolSettings::UIToolSettings(IScene* parentScene, float posX, float posY)
	: IUIPanel(posX, posY), m_ParentScene(parentScene)
{
	ImVec2 windowSize = ImGui::GetMainViewport()->Size;
	ImVec2 windowPos = ImGui::GetMainViewport()->Pos;

	m_Width = windowSize.x / 3.0f;
	m_Height = windowSize.y - posY + windowPos.y;
	
	LOG_INFO("Initialized tool settings panel");

	// m_Paths.axialPaths = nullptr;
	// m_Paths.coronalPaths = nullptr;
	// m_Paths.sagittalPaths = nullptr;
}

//void UIToolSettings::LoadBrushPaths(const PathsPack& paths)
//{
//	m_Paths.axialPaths = paths.axialPaths;
//	m_Paths.coronalPaths = paths.coronalPaths;
//	m_Paths.sagittalPaths = paths.sagittalPaths;
//}

void UIToolSettings::Render()
{
	/*ImGui::SetNextWindowSize(ImVec2(m_Width, m_Height));
	ImGui::SetNextWindowPos(ImVec2(m_PosX, m_PosY));

	ImGui::Begin("Tool settings", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	Update();*/

	//// Temporary before we have a proper tool class and tool settings subclass
	//ImGui::ColorPicker3("Brush color", Path::s_Color, ImGuiColorEditFlags_PickerHueWheel);

	//ImGui::Text("Current brushes");

	//if (m_Paths.axialPaths && ImGui::CollapsingHeader("Axial"))
	//{
	//	for (auto& path : *(m_Paths.axialPaths))
	//	{
	//		ImGui::Checkbox(std::format("Axial path #{}", path.pathID).c_str(), &path.shoudlDraw);
	//	}
	//}

	//if (m_Paths.coronalPaths && ImGui::CollapsingHeader("Coronal"))
	//{
	//	for (auto& path : *(m_Paths.coronalPaths))
	//	{
	//		ImGui::Checkbox(std::format("Coronal path #{}", path.pathID).c_str(), &path.shoudlDraw);
	//	}
	//}

	//if (m_Paths.sagittalPaths && ImGui::CollapsingHeader("Sagittal"))
	//{
	//	for (auto& path : *(m_Paths.sagittalPaths))
	//	{
	//		ImGui::Checkbox(std::format("Sagittal path #{}", path.pathID).c_str(), &path.shoudlDraw);
	//	}
	//}

	//ImGui::End();
}
