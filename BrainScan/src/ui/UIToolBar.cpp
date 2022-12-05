#include "UIToolBar.hpp"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"
#include "../OpenGL.hpp"
#include "../Core.hpp"

// Temporary
static Texture* text = nullptr;

UIToolBarButton::UIToolBarButton(int id, std::function<void(void)> func)
	: m_OnClick(func), m_ID(id)
{}

void UIToolBarButton::Render(ImVec2 uv0, ImVec2 uv1)
{
	if(ImGui::ImageButtonEx(m_ID, (void*)(GLuint)text->GetID(), ImVec2(ImFloor(ImGui::GetFontSize()), ImFloor(ImGui::GetFontSize())), uv0, uv1, ImVec4(0, 0, 0, 0), ImVec4(1, 1, 1, 1)))
	{
		m_OnClick();
	}
}

UIToolBar::UIToolBar(float posX, float posY)
	: IUIPanel(posX, posY)
{
	ImVec2 windowSize = ImGui::GetMainViewport()->Size;

	m_Width  = windowSize.x;
	m_Height = 46.0f;

	LOG_INFO("Initialized tool bar panel");
}

static void PopulateButtons(ImVec2 iconSize, std::vector<UIToolBarButton>& buttons)
{
	if(!text)
	{
		text = new Texture("res/textures/tools_atlas.png");
	}

	for(int i = 0; i < buttons.size(); ++i)
	{
		if(i > 0)
			ImGui::SameLine();

		text->Bind();
		// ImGui::ImageButton((void*)(GLuint)text->GetID(), ImVec2(ImFloor(ImGui::GetFontSize()), ImFloor(ImGui::GetFontSize())), ImVec2(icon_n / 10.0f, 1.0f), ImVec2((icon_n + 1) / 10.0f, 0.0f));
		buttons[i].Render(ImVec2(i / 10.0f, 1.0f), ImVec2((i + 1) / 10.0f, 0.0f));
	}
}

void UIToolBar::AddButton(std::function<void(void)> f)
{
	m_Buttons.emplace_back(m_Buttons.size() + 1, f);
}

void UIToolBar::Render()
{
	ImVec2 windowPos = ImGui::GetMainViewport()->Pos;

	ImGui::SetNextWindowSize(ImVec2(ImGui::GetIO().DisplaySize.x, 0.0f));
	ImGui::SetNextWindowPos(ImVec2(windowPos.x, windowPos.y + 30.0f));

	ImGuiWindowClass window;

	window.DockingAllowUnclassed = true;
	window.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoCloseButton;
	window.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_HiddenTabBar;
	window.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingSplitMe;
	window.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverMe;
	window.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoDockingOverOther;
	window.DockNodeFlagsOverrideSet |= ImGuiDockNodeFlags_NoResizeY;

	ImGui::SetNextWindowClass(&window);

	float fontSize = ImGui::GetFontSize();
	ImVec2 iconSize(ImFloor(fontSize * 1.7f), ImFloor(fontSize * 1.7f));

	ImGui::Begin("Toolbar", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar);

	Update();

	ImGuiDockNode* node = ImGui::GetWindowDockNode();

	if(node)
	{
		ImGuiStyle& style = ImGui::GetStyle();
		ImGuiAxis toolbarAxisPerp = (ImGuiAxis)(ImGuiAxis_X ^ 1);

		float TOOLBAR_SIZE_WHEN_DOCKED = style.WindowPadding[toolbarAxisPerp] * 2.0f + iconSize[toolbarAxisPerp];

		node->WantLockSizeOnce = true;
		node->Size[toolbarAxisPerp] = node->SizeRef[toolbarAxisPerp] = TOOLBAR_SIZE_WHEN_DOCKED;
	}
	
	ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(5.0f, 5.0f));

	PopulateButtons(iconSize, m_Buttons);

	ImGui::PopStyleVar(1);
	ImGui::End();
}
