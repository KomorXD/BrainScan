#pragma once

#include "../scenes/IScene.hpp"
#include "../ui/UIToolSettings.hpp"

class ITool 
{
	protected:
		IScanScene* m_ParentScene = nullptr;

	public:
		ITool(IScanScene* parentScene)
			: m_ParentScene(parentScene) {}
		
		virtual ~ITool() = default;

		virtual std::unique_ptr<UIToolSettings> GetSettingsUI(float posX, float posY) = 0;
		virtual void Update() = 0;
		virtual void UseTool() = 0;
};