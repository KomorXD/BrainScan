#include "AdvancedScene.hpp"
#include "imgui/imgui.h"

#include <iostream>

void AdvancedScene::Input()
{

}

void AdvancedScene::Update()
{}

void AdvancedScene::Render()
{
	ImGui::Begin("Window 1");
	ImGui::Text("Placeholder 1");

	if (ImGui::Button("Placeholder button 1"))
	{
		std::cout << "advanced xd 1\n";
	}

	ImGui::End();

	ImGui::Begin("Window 2");
	ImGui::Text("Placeholder 2");

	if (ImGui::Button("Placeholder button 2"))
	{
		std::cout << "advanced xd 2\n";
	}

	ImGui::End();

	ImGui::Begin("Window 3");
	ImGui::Text("Placeholder 3");

	if (ImGui::Button("Placeholder button 3"))
	{
		std::cout << "advanced xd 3\n";
	}

	ImGui::End();

	ImGui::Begin("Window 4");
	ImGui::Text("Placeholder 4");

	if (ImGui::Button("Placeholder button 4"))
	{
		std::cout << "advanced xd 4\n";
	}

	ImGui::End();
}

void AdvancedScene::SetTool()
{}
