#pragma once

#include "ITool.hpp"

class ToolMask : public ITool
{
	private:
		std::shared_ptr<Shader> m_ScanShader;

		float m_MaskMin = 0.0f;
		float m_MaskMax = 1.0f;

	public:
		ToolMask(IScanScene* parentScene, std::shared_ptr<Shader>& scanShader);
		virtual ~ToolMask();

		virtual std::unique_ptr<UIToolSettings> GetSettingsUI(float posX, float posY) override;
		virtual void Update() override;
		virtual void UseTool() override;
};