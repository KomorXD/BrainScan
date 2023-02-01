#pragma once

#include "IUIPanel.hpp"

class IScanScene;

class UIToolSettings : public IUIPanel
{
	protected:
		IScanScene* m_ParentScene = nullptr;

	public:
		UIToolSettings(IScanScene* parentScene, float posX = 0.0f, float posY = 0.0f);
		virtual ~UIToolSettings() = default;

		virtual void Render();
};