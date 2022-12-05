#pragma once

#include "IUIPanel.hpp"

class UIToolSettings : public IUIPanel
{
	public:
		UIToolSettings(float posX = 0.0f, float posY = 0.0f);
		virtual ~UIToolSettings() = default;

		virtual void Render() override;
};