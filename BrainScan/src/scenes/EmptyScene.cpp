#include "EmptyScene.hpp"
#include "imgui/imgui.h"
#include "imgui/ImGuiFileDialog.h"
#include "../Core.hpp"
#include "../App.hpp"
#include "BasicScene.hpp"
#include "AdvancedScene.hpp"

#include "../ui/UIMenuBar.hpp"
#include "../ui/UIToolBar.hpp"
#include "../ui/UIToolSettings.hpp"
#include "../ui/UIScanImageWindow.hpp"


EmptyScene::EmptyScene()
{
	FUNC_PROFILE();

	m_MenuBar = std::make_unique<UIMenuBar>();
	m_Scan = std::make_unique<Scan>();

	//File menu
	m_MenuBar->PushMenu("File");
	m_MenuBar->PushMenuItem("Open", "Ctrl + O", []() 
		{ 
			ImGuiFileDialog::Instance()->OpenDialog("ChooseScan", "Open scan file", ".nii", ".", 1, nullptr, ImGuiFileDialogFlags_Modal);
		});
	
	m_MenuBar->PushMenuItem("Open sample data", "Ctrl + T", [&]()
		{
			m_Scan->CreateMockData();
			m_ShouldShowModal = true;		
		});
	m_MenuBar->PushMenuItem("Save", "Ctrl + S", []() { LOG_INFO("File/Save pressed."); });
	m_MenuBar->PushMenuItem("Exit", "Ctrl + Q", []() { App::GetInstance().SetWindowShouldClose(true); });

	//Help menu
	m_MenuBar->PushMenu("Help");
	m_MenuBar->PushMenuItem("Info", "Ctrl + I", []() { LOG_INFO("Help/Info pressed."); });
}

void EmptyScene::Render()
{
	m_MenuBar->Render();

	ShowChooseFileDialog();
	ShowChooseFileModeModal();
}

void EmptyScene::SetTool()
{
}

void EmptyScene::Input()
{
}

void EmptyScene::Update()
{
}
