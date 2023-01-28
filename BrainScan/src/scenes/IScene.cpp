#include "IScene.hpp"
#include <GLFW/glfw3.h>
#include "../App.hpp"
#include "../tools/ToolBrush.hpp"

IScanScene::IScanScene(std::unique_ptr<Scan>&& scan)
{
	CreateScrollHandler();
	CreateUIElements();
	CreateImageTexture(std::move(scan));
}

void IScanScene::CreateScrollHandler()
{
	glfwSetWindowUserPointer(App::GetInstance().GetWindow(), this);
	glfwSetScrollCallback(App::GetInstance().GetWindow(), [](GLFWwindow* window, double xOffset, double yOffset)
	{
		IScanScene* thisScene = (IScanScene*)glfwGetWindowUserPointer(window);
		thisScene->OnScroll(yOffset);
	});
}

void IScanScene::CreateUIElements()
{
	m_MenuBar = std::make_unique<UIMenuBar>();
	m_ToolBar = std::make_unique<UIToolBar>(m_MenuBar->GetPosX(), m_MenuBar->GetPosY() + m_MenuBar->GetHeight());
	m_ToolSettings = std::make_unique<UIBrushSettings>(this, m_ToolBar->GetPosX(), m_ToolBar->GetPosY() + m_ToolBar->GetHeight());
}

void IScanScene::CreateImageTexture(std::unique_ptr<Scan>&& scan)
{
	m_Shader = std::make_shared<Shader>("res/shaders/TextureShader.vert", "res/shaders/TextureShader.frag");
	m_Scan = std::move(scan);

	float scanPanelHeight = m_ToolSettings->GetHeight() / 2.0f;

	m_AxialWindow = std::make_unique<UIScanImageWindow>("Axial", m_ToolSettings->GetPosX() + m_ToolSettings->GetWidth(), m_ToolSettings->GetPosY(), scanPanelHeight);
	m_AxialWindow->SetView(m_Scan->GetAxial());
	m_AxialWindow->SetShader(m_Shader);

	m_CoronalWindow = std::make_unique<UIScanImageWindow>("Coronal", m_AxialWindow->GetPosX() + m_AxialWindow->GetWidth(), m_AxialWindow->GetPosY(), scanPanelHeight);
	m_CoronalWindow->SetView(m_Scan->GetCoronal());
	m_CoronalWindow->SetShader(m_Shader);

	m_SagittalWindow = std::make_unique<UIScanImageWindow>("Sagittal", m_CoronalWindow->GetPosX() - m_CoronalWindow->GetWidth(), m_CoronalWindow->GetPosY() + m_CoronalWindow->GetHeight(), scanPanelHeight);
	m_SagittalWindow->SetView(m_Scan->GetSagittal());
	m_SagittalWindow->SetShader(m_Shader);

	m_NullWindow = std::make_unique<UIScanImageWindow>("NULL", m_SagittalWindow->GetPosX() + m_SagittalWindow->GetWidth(), m_SagittalWindow->GetPosY(), scanPanelHeight);
	m_NullWindow->SetShader(m_Shader);

	GLCall(glEnable(GL_MULTISAMPLE));
	GLCall(glLineWidth(10.0f));

	UIScanImageWindow::InitializeBuffers((uint32_t)m_AxialWindow->GetWidth() * 3, (uint32_t)m_AxialWindow->GetHeight() * 2);
	Path::InitializeBuffers();
}


PathsPack IScanScene::RequestPathsPack()
{
	PathsPack paths;
	
	paths.axialPaths    = m_AxialWindow->GetBrushPaths();
	paths.coronalPaths  = m_CoronalWindow->GetBrushPaths();
	paths.sagittalPaths = m_SagittalWindow->GetBrushPaths();

	return paths;
}

void IScanScene::Render()
{
	m_MenuBar->Render();
	m_ToolBar->Render();
	m_ToolSettings->Render();

	m_AxialWindow->Render();
	m_CoronalWindow->Render();
	m_SagittalWindow->Render();
	m_NullWindow->Render();
}

void IScanScene::OnScroll(double offset)
{
	ImGui::GetIO().AddMouseWheelEvent(0.0f, (float)offset);

	if (m_AxialWindow->TryToHandleScroll(offset))    return;
	if (m_CoronalWindow->TryToHandleScroll(offset))  return;
	if (m_SagittalWindow->TryToHandleScroll(offset)) return;
}

void IScanScene::Input()
{
}

void IScanScene::Update()
{
	if (!ImGui::GetIO().MouseDown[1])
	{
		return;
	}

	if (m_AxialWindow->IsHovered()) {
		auto[x, y] = m_AxialWindow->GetNormalizedLastMousePos();

		m_CoronalWindow->SetDepthFromNormalizedCoordinate(1.0f - y);
		m_SagittalWindow->SetDepthFromNormalizedCoordinate(x);
	}

	if (m_CoronalWindow->IsHovered()) {
		auto [x, y] = m_CoronalWindow->GetNormalizedLastMousePos();

		m_AxialWindow->SetDepthFromNormalizedCoordinate(y);
		m_SagittalWindow->SetDepthFromNormalizedCoordinate(x);
	}

	if (m_SagittalWindow->IsHovered()) {
		auto [x, y] = m_SagittalWindow->GetNormalizedLastMousePos();

		m_CoronalWindow->SetDepthFromNormalizedCoordinate(x);
		m_AxialWindow->SetDepthFromNormalizedCoordinate(1.0f - y);
	}
}

void IScanScene::SetTool()
{
}
