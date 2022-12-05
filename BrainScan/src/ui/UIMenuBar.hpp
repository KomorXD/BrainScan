#pragma once

#include "IUIPanel.hpp"

class UIMenuBar : public IUIPanel
{
	public:
		UIMenuBar();
		virtual ~UIMenuBar() = default;

		virtual void Render() override;
};