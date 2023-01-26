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


static bool s_ShouldShowModal = false;
static std::string s_InputImageFileName;

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
	m_MenuBar->PushMenuItem("Save", "Ctrl + S", []() { LOG_INFO("File/Save pressed."); });
	m_MenuBar->PushMenuItem("Exit", "Ctrl + Q", []() { App::GetInstance().SetWindowShouldClose(true); });

	//Help menu
	m_MenuBar->PushMenu("Help");
	m_MenuBar->PushMenuItem("Info", "Ctrl + I", []() { LOG_INFO("Help/Info pressed."); });
}

void EmptyScene::Render()
{
	m_MenuBar->Render();

	if (ImGuiFileDialog::Instance()->Display("ChooseScan", 32, ImVec2(600.0f, 400.0f)))
	{

		if (ImGuiFileDialog::Instance()->IsOk())
		{
			s_InputImageFileName = ImGuiFileDialog::Instance()->GetFilePathName();
			if (m_Scan->LoadFromFile(s_InputImageFileName))
			{
				s_ShouldShowModal = true;
			}
			else
			{
				s_ShouldShowModal = false;
			}
		}

		ImGuiFileDialog::Instance()->Close();
	}

	if (s_ShouldShowModal)
	{
		ImGui::OpenPopup("Choose mode");

		if (ImGui::BeginPopupModal("Choose mode", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("In what mode should the file be opened?\n\n");
			ImGui::Separator();

			if (ImGui::Button("User", ImVec2(120, 0))) 
			{
				ImGui::CloseCurrentPopup();
				s_ShouldShowModal = false;
				App::GetInstance().SetNextScene(std::make_unique<BasicScene>(std::move(m_Scan)));
			}

			ImGui::SameLine();

			if (ImGui::Button("Admin", ImVec2(120, 0))) 
			{
				ImGui::CloseCurrentPopup();
				s_ShouldShowModal = false;
				App::GetInstance().SetNextScene(std::make_unique<AdvancedScene>(std::move(m_Scan)));
			}

			ImGui::EndPopup();
		}
	}
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
