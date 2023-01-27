#include "UIToolSettings.hpp"
#include "imgui/imgui.h"
#include "../Core.hpp"
#include "../scenes/IScene.hpp"

UIToolSettings::UIToolSettings(IScanScene* parentScene, float posX, float posY)
	: IUIPanel(posX, posY), m_ParentScene(parentScene)
{
	ImVec2 windowSize = ImGui::GetMainViewport()->Size;
	ImVec2 windowPos = ImGui::GetMainViewport()->Pos;

	m_Width = windowSize.x / 3.0f;
	m_Height = windowSize.y - posY + windowPos.y;
	
	LOG_INFO("Initialized tool settings panel");
}

void UIToolSettings::Render()
{

}
