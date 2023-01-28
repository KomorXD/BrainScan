#pragma once

#include "UIToolSettings.hpp"
#include "UIScanImageWindow.hpp"

class UIMaskSettings : public UIToolSettings
{
	private:
		float* m_MaskMin = nullptr;
		float* m_MaskMax = nullptr;

	public:
		UIMaskSettings(IScanScene* parentScene, float* maskMin, float* maskMax, float posX = 0.0f, float posY = 0.0f);
		virtual ~UIMaskSettings() = default;
		
		virtual void Render() override;
};