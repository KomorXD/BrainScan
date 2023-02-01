#include "ToolMask.hpp"
#include "../ui/UIMaskSettings.hpp"

float ToolMask::s_MaskMin = 0.0f;
float ToolMask::s_MaskMax = 1.0f;

ToolMask::ToolMask(IScanScene* parentScene, std::shared_ptr<Shader>& scanShader)
	: ITool(parentScene), m_ScanShader(scanShader)
{
}

ToolMask::~ToolMask()
{
}

std::unique_ptr<UIToolSettings> ToolMask::GetSettingsUI(float posX, float posY)
{
	return std::make_unique<UIMaskSettings>(m_ParentScene, &s_MaskMin, &s_MaskMax, posX, posY);
}

void ToolMask::Update()
{
	m_ScanShader->Bind();
	m_ScanShader->SetUniform1f("u_MaskMin", s_MaskMin);
	m_ScanShader->SetUniform1f("u_MaskMax", s_MaskMax);
}

void ToolMask::UseTool()
{
}
