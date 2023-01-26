#include "BasicScene.hpp"
#include "imgui/imgui.h"
#include "../Core.hpp"
#include "../App.hpp"

BasicScene::BasicScene(std::unique_ptr<Scan>&& scan)
{
	FUNC_PROFILE();

	m_Scan = std::move(scan);
	m_Shader = std::make_shared<Shader>("res/shaders/TextureShader.vert", "res/shaders/TextureShader.frag");

	//File menu
	m_MenuBar = std::make_unique<UIMenuBar>();
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

	//Toolbar
	m_ToolBar = std::make_unique<UIToolBar>(m_MenuBar->GetPosX(), m_MenuBar->GetPosY() + m_MenuBar->GetHeight());
	m_ToolBar->AddButton([]() { LOG_INFO("Button 1 pressed."); });

	m_CurrentTool = std::make_unique<ToolBrush>(this);
	m_ToolSettings = std::make_unique<UIBrushSettings>(this, m_ToolBar->GetPosX(), m_ToolBar->GetPosY() + m_ToolBar->GetHeight());

	View* axial = m_Scan->GetAxial();
	View* coronal = m_Scan->GetCoronal();
	View* sagittal = m_Scan->GetSagittal();

	float scanPanelHeight = m_ToolSettings->GetHeight() / 2.0f;

	m_ScanWindows.reserve(4);

	m_ScanWindows.emplace_back("Axial", m_ToolSettings->GetPosX() + m_ToolSettings->GetWidth(), m_ToolSettings->GetPosY(), scanPanelHeight);
	m_ScanWindows.back().SetView(axial);
	m_ScanWindows.back().SetShader(m_Shader);

	m_ScanWindows.emplace_back("Coronal", m_ScanWindows.back().GetPosX() + m_ScanWindows.back().GetWidth(), m_ScanWindows.back().GetPosY(), scanPanelHeight);
	m_ScanWindows.back().SetView(coronal);
	m_ScanWindows.back().SetShader(m_Shader);

	m_ScanWindows.emplace_back("Sagittal", m_ScanWindows.back().GetPosX() - m_ScanWindows.back().GetWidth(), m_ScanWindows.back().GetPosY() + m_ScanWindows.back().GetHeight(), scanPanelHeight);
	m_ScanWindows.back().SetView(sagittal);
	m_ScanWindows.back().SetShader(m_Shader);

	m_ScanWindows.emplace_back("NULL", m_ScanWindows.back().GetPosX() + m_ScanWindows.back().GetWidth(), m_ScanWindows.back().GetPosY(), scanPanelHeight);
	m_ScanWindows.back().SetShader(m_Shader);

	GLCall(glEnable(GL_MULTISAMPLE));
	GLCall(glLineWidth(10.0f));

	UIScanImageWindow::InitializeBuffers((uint32_t)m_ScanWindows.back().GetWidth() * 3, (uint32_t)m_ScanWindows.back().GetHeight() * 2);
	Path::InitializeBuffers();
}

PathsPack BasicScene::RequestPathsPack()
{
	PathsPack paths;

	paths.axialPaths    = m_ScanWindows[0].GetBrushPaths();
	paths.coronalPaths  = m_ScanWindows[1].GetBrushPaths();
	paths.sagittalPaths = m_ScanWindows[2].GetBrushPaths();

	return paths;
}

void BasicScene::Render()
{
	m_MenuBar->Render();
	m_ToolBar->Render();
	m_ToolSettings->Render();

	for (auto& scanWindow : m_ScanWindows)
	{
		scanWindow.Render();
	}
}

void BasicScene::SetTool()
{

}

void BasicScene::Input()
{

}

void BasicScene::Update()
{

}