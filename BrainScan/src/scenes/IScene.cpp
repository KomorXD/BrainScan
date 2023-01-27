#include "IScene.hpp"
#include <GLFW/glfw3.h>
#include "../App.hpp"
#include "../tools/ToolBrush.hpp"

IScanScene::IScanScene(std::unique_ptr<Scan>&& scan)
{
	glfwSetWindowUserPointer(App::GetInstance().GetWindow(), this);
	glfwSetScrollCallback(App::GetInstance().GetWindow(), [](GLFWwindow* window, double xOffset, double yOffset)
		{
			IScanScene* thisScene = (IScanScene*)glfwGetWindowUserPointer(window);
			thisScene->OnScroll(yOffset);
		});

	m_Scan = std::move(scan);
	m_Shader = std::make_shared<Shader>("res/shaders/TextureShader.vert", "res/shaders/TextureShader.frag");

	m_MenuBar = std::make_unique<UIMenuBar>();
	m_ToolBar = std::make_unique<UIToolBar>(m_MenuBar->GetPosX(), m_MenuBar->GetPosY() + m_MenuBar->GetHeight());

	View* axial = m_Scan->GetAxial();
	View* coronal = m_Scan->GetCoronal();
	View* sagittal = m_Scan->GetSagittal();

	m_ToolSettings = std::make_unique<UIBrushSettings>(this, m_ToolBar->GetPosX(), m_ToolBar->GetPosY() + m_ToolBar->GetHeight());
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

PathsPack IScanScene::RequestPathsPack()
{
	PathsPack paths;

	paths.axialPaths = m_ScanWindows[0].GetBrushPaths();
	paths.coronalPaths = m_ScanWindows[1].GetBrushPaths();
	paths.sagittalPaths = m_ScanWindows[2].GetBrushPaths();

	return paths;
}

void IScanScene::Render()
{
	m_MenuBar->Render();
	m_ToolBar->Render();
	m_ToolSettings->Render();

	for (auto& scanWindow : m_ScanWindows)
	{
		scanWindow.Render();
	}
}

void IScanScene::OnScroll(double offset)
{
	ImGui::GetIO().AddMouseWheelEvent(0.0f, (float)offset);

	for (auto& scanWindow : m_ScanWindows)
	{
		if (scanWindow.TryToHandleScroll(offset))
		{
			break;
		}
	}
}
