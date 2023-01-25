#include "AdvancedScene.hpp"
#include "imgui/imgui.h"
#include "../Core.hpp"

AdvancedScene::AdvancedScene(const std::string& inputImageFileName)
{
	FUNC_PROFILE();

	m_Scan.LoadFromFile(inputImageFileName);

	m_Shader = std::make_shared<Shader>("res/shaders/TextureShader.vert", "res/shaders/TextureShader.frag");
	m_AxialTexture = std::make_shared<Texture>(m_Scan.GetAxial()->GetData()[40], m_Scan.GetAxial()->GetWidth(), m_Scan.GetAxial()->GetHeight());
	m_CoronalTexture = std::make_shared<Texture>(m_Scan.GetCoronal()->GetData()[40], m_Scan.GetCoronal()->GetWidth(), m_Scan.GetCoronal()->GetHeight());
	m_SagittalTexture = std::make_shared<Texture>(m_Scan.GetSagittal()->GetData()[40], m_Scan.GetSagittal()->GetWidth(), m_Scan.GetSagittal()->GetHeight());

	m_MenuBar = std::make_unique<UIMenuBar>();
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

void AdvancedScene::Input()
{
	// TODO: handle user input
}

void AdvancedScene::Update()
{
	PathsPack paths;

	paths.axialPaths = m_ScanWindows[0].GetBrushPaths();
	paths.coronalPaths = m_ScanWindows[1].GetBrushPaths();
	paths.sagittalPaths = m_ScanWindows[2].GetBrushPaths();

	m_ToolSettings->LoadBrushPaths(paths);
}

void AdvancedScene::Render()
{
	m_MenuBar->Render();
	m_ToolBar->Render();
	m_ToolSettings->Render();

	for (auto& scanWindow : m_ScanWindows)
	{
		scanWindow.Render();
	}
}

void AdvancedScene::SetTool()
{
	// TODO: implement xd
}
