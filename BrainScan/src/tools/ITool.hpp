#pragma once

#include "../scenes/IScene.hpp"
#include "../ui/UIToolSettings.hpp"

class ITool 
{
	protected:
		IScene* m_ParentScene = nullptr;

	public:
		ITool(IScene* parentScene)
			: m_ParentScene(parentScene) {}
		
		virtual ~ITool() = default;

		virtual std::unique_ptr<UIToolSettings> GetSettingsUI(float posX, float posY) = 0;
		virtual void UseTool() = 0;
};