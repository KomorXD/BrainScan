#include "BasicScene.hpp"
#include "imgui/imgui.h"
#include "../Core.hpp"
#include "../App.hpp"

BasicScene::BasicScene(const std::string& inputImageFileName)
{
	FUNC_PROFILE();

	m_Scan.LoadFromFile(inputImageFileName);

	m_Shader = std::make_shared<Shader>("res/shaders/TextureShader.vert", "res/shaders/TextureShader.frag");
	m_AxialTexture = std::make_shared<Texture>(m_Scan.GetAxial()->GetData()[40], m_Scan.GetAxial()->GetWidth(), m_Scan.GetAxial()->GetHeight());
	m_CoronalTexture = std::make_shared<Texture>(m_Scan.GetCoronal()->GetData()[40], m_Scan.GetCoronal()->GetWidth(), m_Scan.GetCoronal()->GetHeight());
	m_SagittalTexture = std::make_shared<Texture>(m_Scan.GetSagittal()->GetData()[40], m_Scan.GetSagittal()->GetWidth(), m_Scan.GetSagittal()->GetHeight());

	m_MenuBar = std::make_unique<UIMenuBar>();

	m_MenuBar->PushMenu("File");
	m_MenuBar->PushMenuItem("Open", "Ctrl + O", []() { LOG_INFO("File/Open pressed."); });
	m_MenuBar->PushMenuItem("Save", "Ctrl + S", []() { LOG_INFO("File/Save pressed."); });
	m_MenuBar->PushMenuItem("Exit", "Ctrl + Q", []() { App::GetInstance().SetWindowShouldClose(true); });

	m_MenuBar->PushMenu("Options");
	m_MenuBar->PushMenuItem("nwm1", "Ctrl + 1", []() { LOG_INFO("Options/nwm1 pressed."); });
	m_MenuBar->PushMenuItem("nwm2", "Ctrl + 2", []() { LOG_INFO("Options/nwm2 pressed."); });
	m_MenuBar->PushMenuItem("nwm3", "Ctrl + 3", []() { LOG_INFO("Options/nwm3 pressed."); });

	m_MenuBar->PushMenu("Help");
	m_MenuBar->PushMenuItem("Info", "Ctrl + I", []() { LOG_INFO("jest sroda moje ziomki"); });

	m_ToolBar = std::make_unique<UIToolBar>(m_MenuBar->GetPosX(), m_MenuBar->GetPosY() + m_MenuBar->GetHeight());

	for (int i = 0; i < 10; ++i)
	{
		m_ToolBar->AddButton([i]()
			{
				LOG_INFO("Button #{} pressed.", i + 1);
			});
	}

	m_ToolSettings = std::make_unique<UIToolSettings>(m_ToolBar->GetPosX(), m_ToolBar->GetPosY() + m_ToolBar->GetHeight());

	float scanPanelHeight = m_ToolSettings->GetHeight() / 2.0f;
	uint32_t sizeX = 0;
	uint32_t sizeY = 0;

	m_ScanWindows.reserve(4);

	sizeX = m_Scan.GetAxial()->GetWidth();
	sizeY = m_Scan.GetAxial()->GetHeight();
	m_ScanWindows.emplace_back(1, m_ToolSettings->GetPosX() + m_ToolSettings->GetWidth(), m_ToolSettings->GetPosY(), scanPanelHeight);
	m_ScanWindows.back().SetScanTexture(m_AxialTexture);
	m_ScanWindows.back().SetShader(m_Shader);

	m_ScanWindows.emplace_back(2, m_ScanWindows.back().GetPosX() + m_ScanWindows.back().GetWidth(), m_ScanWindows.back().GetPosY(), scanPanelHeight);
	m_ScanWindows.back().SetScanTexture(m_CoronalTexture);
	m_ScanWindows.back().SetShader(m_Shader);

	m_ScanWindows.emplace_back(3, m_ScanWindows.back().GetPosX() - m_ScanWindows.back().GetWidth(), m_ScanWindows.back().GetPosY() + m_ScanWindows.back().GetHeight(), scanPanelHeight);
	m_ScanWindows.back().SetScanTexture(m_SagittalTexture);
	m_ScanWindows.back().SetShader(m_Shader);

	m_ScanWindows.emplace_back(4, m_ScanWindows.back().GetPosX() + m_ScanWindows.back().GetWidth(), m_ScanWindows.back().GetPosY(), scanPanelHeight);
	m_ScanWindows.back().SetScanTexture(m_AxialTexture);
	m_ScanWindows.back().SetShader(m_Shader);

	GLCall(glEnable(GL_MULTISAMPLE));
	GLCall(glLineWidth(10.0f));

	UIScanImageWindow::InitializeBuffers((uint32_t)m_ScanWindows.back().GetWidth() * 3, (uint32_t)m_ScanWindows.back().GetHeight() * 2);
	Path::InitializeBuffers();
}

void BasicScene::Input()
{

}

void BasicScene::Update()
{

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
