#include "ToolMask.hpp"
#include "../ui/UIMaskSettings.hpp"

ToolMask::ToolMask(IScanScene* parentScene, std::shared_ptr<Shader>& scanShader)
	: ITool(parentScene), m_ScanShader(scanShader)
{
}

ToolMask::~ToolMask()
{
}

std::unique_ptr<UIToolSettings> ToolMask::GetSettingsUI(float posX, float posY)
{
	return std::make_unique<UIMaskSettings>(m_ParentScene, &m_MaskMin, &m_MaskMax, posX, posY);
}

void ToolMask::Update()
{
	m_ScanShader->Bind();
	m_ScanShader->SetUniform1f("u_MaskMin", m_MaskMin);
	m_ScanShader->SetUniform1f("u_MaskMax", m_MaskMax);
}

void ToolMask::UseTool()
{
}
