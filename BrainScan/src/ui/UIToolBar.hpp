#pragma once

#include "IUIPanel.hpp"
#include "imgui/imgui.h"
#include "../OpenGL.hpp"

#include <functional>
#include <vector>

class UIToolBarButton
{
	private:
		std::function<void(void)> m_OnClick = []() {};
		int m_ID;

	public:
		UIToolBarButton(int id, const std::function<void(void)>& func);

		void Render(void* textID, ImVec2 uv0, ImVec2 uv1) const;
};

class UIToolBar : public IUIPanel
{
	private:
		std::vector<UIToolBarButton> m_Buttons;

		Texture m_ToolBarTexture;

	public:
		UIToolBar(float posX = 0.0f, float posY = 0.0f);
		virtual ~UIToolBar() = default;

		void AddButton(std::function<void(void)> f);

		virtual void Render() override;

	private:
		void RenderButtons();
};