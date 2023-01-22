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
	m_Panels.emplace_back(std::make_unique<UIMenuBar>());
	m_Panels.emplace_back(std::make_unique<UIToolBar>(m_Panels.back()->GetPosX(), m_Panels.back()->GetPosY() + m_Panels.back()->GetHeight()));

	auto tb = (UIToolBar*)m_Panels.back().get();

	tb->AddButton([]()
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

static bool shouldShowModal = false;

void EmptyScene::Render()
{
	for(const auto& panel : m_Panels)
	{
		panel->Render();
	}

	if (ImGuiFileDialog::Instance()->Display("ChooseScan1", 32, ImVec2(600.0f, 400.0f)))
	{

		if (ImGuiFileDialog::Instance()->IsOk())
		{
			std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
			shouldShowModal = true;
		}

		ImGuiFileDialog::Instance()->Close();
	}

	if (shouldShowModal)
	{
		ImGui::OpenPopup("Choose mode");

		if (ImGui::BeginPopupModal("Choose mode", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("In what mode should the file be opened?\n\n");
			ImGui::Separator();

			if (ImGui::Button("User", ImVec2(120, 0))) 
			{
				ImGui::CloseCurrentPopup();
				shouldShowModal = false;
				App::GetInstance().SetNextScene(std::make_unique<BasicScene>());
			}

			ImGui::SameLine();

			if (ImGui::Button("Admin", ImVec2(120, 0))) 
			{
				ImGui::CloseCurrentPopup();
				shouldShowModal = false;
				App::GetInstance().SetNextScene(std::make_unique<AdvancedScene>());
			}

			ImGui::EndPopup();
		}
	}

}

void EmptyScene::SetTool()
{
	// TODO: implement xd
}
