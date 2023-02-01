#pragma once

#include "ITool.hpp"

class ToolNone : public ITool
{
	public:
		ToolNone(IScanScene* parentScene);
		virtual ~ToolNone();

		virtual std::unique_ptr<UIToolSettings> GetSettingsUI(float posX, float posY) override;
		virtual void Update() override {}
		virtual void UseTool() override;
};