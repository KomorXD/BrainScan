#include "AdvancedScene.hpp"
#include "imgui/imgui.h"
#include "../Core.hpp"
#include "../App.hpp"
#include "../tools/ToolBrush.hpp"

AdvancedScene::AdvancedScene(std::unique_ptr<Scan>&& scan) : IScanScene(std::move(scan))
{
	m_MenuBar->PushMenu("File");
	m_MenuBar->PushMenuItem("Open", "Ctrl + O", []() { LOG_INFO("File/Open pressed."); });
	m_MenuBar->PushMenuItem("Save", "Ctrl + S", []() { LOG_INFO("File/Save pressed."); });
	m_MenuBar->PushMenuItem("Exit", "Ctrl + Q", []() { App::GetInstance().SetWindowShouldClose(true); });

	m_MenuBar->PushMenu("Options");
	m_MenuBar->PushMenuItem("Reload shaders", "", [this]() { m_Shader->ReloadShader(); Path::s_PathsShader->ReloadShader(); });

	m_MenuBar->PushMenu("Help");
	m_MenuBar->PushMenuItem("Info", "Ctrl + I", []() { LOG_INFO("Help/Info."); });

	for (int i = 0; i < 8; ++i)
	{
		m_ToolBar->AddButton([i]()
			{
				LOG_INFO("Button #{} pressed.", i + 1);
			});
	}

	m_CurrentTool = std::make_unique<ToolBrush>(this);
}

void AdvancedScene::Input()
{
}
void AdvancedScene::Update()
{
}
void AdvancedScene::SetTool()
{
}