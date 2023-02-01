#include "ToolNone.hpp"

ToolNone::ToolNone(IScanScene* parentScene)
	: ITool(parentScene)
{
}

ToolNone::~ToolNone()
{
}

std::unique_ptr<UIToolSettings> ToolNone::GetSettingsUI(float posX, float posY)
{
    return std::make_unique<UIToolSettings>(m_ParentScene, posX, posY);
}

void ToolNone::UseTool()
{
}
