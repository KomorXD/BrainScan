#pragma once

#include "IUIPanel.hpp"

#include <functional>
#include <string>

struct MenuItem
{
	std::string label{};
	std::string shortcut{};
	std::function<void(void)> func = []() {};
};

struct Menu
{
	std::string label{};
	std::vector<MenuItem> items;
};

class UIMenuBar : public IUIPanel
{
	private:
		std::vector<Menu> m_Menus;

	public:
		UIMenuBar();
		virtual ~UIMenuBar() = default;

		void PushMenu(const std::string& label);
		void PushMenuItem(const std::string& label, const std::string& shortcut, const std::function<void(void)>& f = []() {});

		virtual void Render() override;
};