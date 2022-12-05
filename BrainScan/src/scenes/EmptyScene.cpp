#include "EmptyScene.hpp"
#include "imgui/imgui.h"
#include "../Core.hpp"

#include "../ui/UIMenuBar.hpp"
#include "../ui/UIToolBar.hpp"
#include "../ui/UIToolSettings.hpp"
#include "../ui/UIScanImageWindow.hpp"

EmptyScene::EmptyScene()
{
	FUNC_PROFILE();

	m_Panels.emplace_back(std::make_unique<UIMenuBar>());
	m_Panels.emplace_back(std::make_unique<UIToolBar>(m_Panels.back()->GetPosX(), m_Panels.back()->GetPosY() + m_Panels.back()->GetHeight()));

	for(int i = 0; i < 10; ++i)
	{
		UIToolBar* tb = (UIToolBar*)m_Panels.back().get();

		tb->AddButton([=]()
		{
			LOG_INFO("Button #{} pressed.", i + 1);
		});
	}

	m_Panels.emplace_back(std::make_unique<UIToolSettings>(m_Panels.back()->GetPosX(), m_Panels.back()->GetPosY() + m_Panels.back()->GetHeight()));

	float scanPanelHeight = m_Panels.back()->GetHeight() / 2.0f;

	m_Panels.emplace_back(std::make_unique<UIScanImageWindow>(1, m_Panels.back()->GetPosX() + m_Panels.back()->GetWidth(), m_Panels.back()->GetPosY(),								  scanPanelHeight));
	m_Panels.emplace_back(std::make_unique<UIScanImageWindow>(2, m_Panels.back()->GetPosX() + m_Panels.back()->GetWidth(), m_Panels.back()->GetPosY(),								  scanPanelHeight));
	m_Panels.emplace_back(std::make_unique<UIScanImageWindow>(3, m_Panels.back()->GetPosX() - m_Panels.back()->GetWidth(), m_Panels.back()->GetPosY() + m_Panels.back()->GetHeight(), scanPanelHeight));
	m_Panels.emplace_back(std::make_unique<UIScanImageWindow>(4, m_Panels.back()->GetPosX() + m_Panels.back()->GetWidth(), m_Panels.back()->GetPosY(),								  scanPanelHeight));	
}

void EmptyScene::Input()
{
	
}

void EmptyScene::Update()
{}

void EmptyScene::Render()
{
	for(const auto& panel : m_Panels)
	{
		panel->Render();
	}
}

void EmptyScene::SetTool()
{}
