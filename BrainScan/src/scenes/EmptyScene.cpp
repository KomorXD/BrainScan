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

	m_MenuBar->PushMenu("File");
	m_MenuBar->PushMenuItem("Open", "Ctrl + O", []() { LOG_INFO("File/Open pressed."); });
	m_MenuBar->PushMenuItem("Save", "Ctrl + S", []() { LOG_INFO("File/Save pressed."); });
	m_MenuBar->PushMenuItem("Exit", "Ctrl + Q", []() { App::GetInstance().SetWindowShouldClose(true); });

	m_MenuBar->PushMenu("Help");
	m_MenuBar->PushMenuItem("Info", "Ctrl + I", []() { LOG_INFO("jest sroda moje ziomki"); });

	m_ToolBar = std::make_unique<UIToolBar>(m_MenuBar->GetPosX(), m_MenuBar->GetPosY() + m_MenuBar->GetHeight());

	m_ToolBar->AddButton([]()
	{
		//After clicking the open button, a modal with file system appears
		ImGuiFileDialog::Instance()->OpenDialog("ChooseScan1", "Open scan file", ".nii", ".", 1, nullptr, ImGuiFileDialogFlags_Modal);
		
		LOG_INFO("Button #{} pressed.", 1);
	});
}

void EmptyScene::Input()
{
	// TODO: handle user input
}

void EmptyScene::Update()
{
	// TODO: idk actually
}

static bool s_ShouldShowModal = false;
static std::string s_InputImageFileName;

void EmptyScene::Render()
{
	m_MenuBar->Render();
	m_ToolBar->Render();

	if (ImGuiFileDialog::Instance()->Display("ChooseScan1", 32, ImVec2(600.0f, 400.0f)))
	{

		if (ImGuiFileDialog::Instance()->IsOk())
		{
			s_InputImageFileName = ImGuiFileDialog::Instance()->GetFilePathName();
			s_ShouldShowModal = true;
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
				App::GetInstance().SetNextScene(std::make_unique<BasicScene>(s_InputImageFileName));
			}

			ImGui::SameLine();

			if (ImGui::Button("Admin", ImVec2(120, 0))) 
			{
				ImGui::CloseCurrentPopup();
				s_ShouldShowModal = false;
				App::GetInstance().SetNextScene(std::make_unique<AdvancedScene>(s_InputImageFileName));
			}

			ImGui::EndPopup();
		}
	}

}

void EmptyScene::SetTool()
{
	// TODO: implement xd
}
