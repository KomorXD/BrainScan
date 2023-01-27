#pragma once

#include "ITool.hpp"

class ToolBrush : public ITool
{
	public:
		ToolBrush(IScanScene* parentScene);
		virtual ~ToolBrush();

		virtual std::unique_ptr<UIToolSettings> GetSettingsUI(float posX, float posY) override;
		virtual void UseTool() override;
};