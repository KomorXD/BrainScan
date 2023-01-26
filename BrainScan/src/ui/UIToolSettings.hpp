#pragma once

#include "IUIPanel.hpp"

class IScene;

class UIToolSettings : public IUIPanel
{
	protected:
		IScene* m_ParentScene = nullptr;

	public:
		UIToolSettings(IScene* parentScene, float posX = 0.0f, float posY = 0.0f);
		virtual ~UIToolSettings() = default;

		virtual void Render() = 0;
};