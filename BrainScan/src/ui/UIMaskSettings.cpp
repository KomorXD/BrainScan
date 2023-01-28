#include "UIMaskSettings.hpp"
#include "../Core.hpp"

UIMaskSettings::UIMaskSettings(IScanScene* parentScene, float* maskMin, float* maskMax, float posX, float posY)
	: UIToolSettings(parentScene, posX, posY), m_MaskMin(maskMin), m_MaskMax(maskMax)
{
	LOG_INFO("Initialized mask settings panel");
}

void UIMaskSettings::Render()
{
	ImGui::SetNextWindowSize(ImVec2(m_Width, m_Height));
	ImGui::SetNextWindowPos(ImVec2(m_PosX, m_PosY));

	ImGui::Begin("Mask settings", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	ImGui::SliderFloat("Lower bound", m_MaskMin, 0.0, *m_MaskMax);
	ImGui::NewLine();
	ImGui::SliderFloat("Upper bound", m_MaskMax, *m_MaskMin, 1.0);

	ImGui::End();
}
