#include "BasicScene.hpp"
#include "imgui/imgui.h"
#include "../Core.hpp"
#include "../App.hpp"
#include "../tools/ToolBrush.hpp"
#include "../tools/ToolMask.hpp"

#include <GLFW/glfw3.h>

BasicScene::BasicScene(std::unique_ptr<Scan>&& scan) : IScanScene(std::move(scan))
{
	PopulateMenu();
	PopulateToolBar();
	m_CurrentTool = std::make_unique<ToolBrush>(this);
}

void BasicScene::PopulateMenu()
{
	//File menu
	m_MenuBar->PushMenu("File");
	m_MenuBar->PushMenuItem("Open", "Ctrl + O", []() { LOG_INFO("File/Open pressed."); });
	m_MenuBar->PushMenuItem("Save", "Ctrl + S", []() { LOG_INFO("File/Save pressed."); });
	m_MenuBar->PushMenuItem("Exit", "Ctrl + Q", []() { App::GetInstance().SetWindowShouldClose(true); });

	//Options menu
	m_MenuBar->PushMenu("Options");
	m_MenuBar->PushMenuItem("Reload shaders", "", [this]() { m_Shader->ReloadShader(); Path::s_PathsShader->ReloadShader(); });

	//Help menu
	m_MenuBar->PushMenu("Help");
	m_MenuBar->PushMenuItem("Info", "Ctrl + I", []() { LOG_INFO("Help/Info pressed"); });
}

void BasicScene::PopulateToolBar()
{
	m_ToolBar->AddButton([this]()
		{
		m_CurrentTool = std::make_unique<ToolMask>(this, m_Shader);
		m_ToolSettings = m_CurrentTool->GetSettingsUI(m_ToolBar->GetPosX(), m_ToolBar->GetPosY() + m_ToolBar->GetHeight());
		}
	);

	m_ToolBar->AddButton([]()
		{
			LOG_INFO("Button {} pressed", 2);
		});
	

	m_ToolBar->AddButton([this]()
		{
			m_Shader->Bind();
			m_Shader->SetUniform1f("u_Flip", 0.0);
			LOG_INFO("Radiological convention");
		});

	m_ToolBar->AddButton([this]()
		{
			m_Shader->Bind();
			m_Shader->SetUniform1f("u_Flip", 1.0);
			LOG_INFO("Neurological convention");
		});

	m_ToolBar->AddButton([]()
		{
			LOG_INFO("Button {} pressed", 5);
		});
}
