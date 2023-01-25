#include "UIBrushSettings.hpp"
#include "../Core.hpp"
#include "../scenes/IScene.hpp"

UIBrushSettings::UIBrushSettings(IScene* paretnScene, float posX, float posY)
	: UIToolSettings(paretnScene, posX, posY)
{
	LOG_INFO("Initialized brush settings panel");
}

void UIBrushSettings::UpdateBrushPaths()
{
	m_Paths = m_ParentScene->RequestPathsPack();
}

void UIBrushSettings::Render()
{
	UpdateBrushPaths();

	ImGui::SetNextWindowSize(ImVec2(m_Width, m_Height));
	ImGui::SetNextWindowPos(ImVec2(m_PosX, m_PosY));

	ImGui::Begin("Brush settings", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	Update();

	// Temporary before we have a proper tool class and tool settings subclass
	ImGui::ColorPicker3("Brush color", Path::s_Color, ImGuiColorEditFlags_PickerHueWheel);

	ImGui::Text("Current brushes");

	if (m_Paths.axialPaths && ImGui::CollapsingHeader("Axial"))
	{
		for (auto& path : *(m_Paths.axialPaths))
		{
			ImGui::Checkbox(std::format("Axial path #{}", path.pathID).c_str(), &path.shoudlDraw);
		}
	}

	if (m_Paths.coronalPaths && ImGui::CollapsingHeader("Coronal"))
	{
		for (auto& path : *(m_Paths.coronalPaths))
		{
			ImGui::Checkbox(std::format("Coronal path #{}", path.pathID).c_str(), &path.shoudlDraw);
		}
	}

	if (m_Paths.sagittalPaths && ImGui::CollapsingHeader("Sagittal"))
	{
		for (auto& path : *(m_Paths.sagittalPaths))
		{
			ImGui::Checkbox(std::format("Sagittal path #{}", path.pathID).c_str(), &path.shoudlDraw);
		}
	}

	ImGui::End();
}
