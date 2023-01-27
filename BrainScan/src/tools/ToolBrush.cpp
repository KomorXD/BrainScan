#include "ToolBrush.hpp"
#include "../ui/UIScanImageWindow.hpp"

bool UIScanImageWindow::s_DrawingEnabled;

ToolBrush::ToolBrush(IScanScene* parentScene)
	: ITool(parentScene)
{
	UIScanImageWindow::s_DrawingEnabled = true;
}

ToolBrush::~ToolBrush()
{
	UIScanImageWindow::s_DrawingEnabled = false;
}

std::unique_ptr<UIToolSettings> ToolBrush::GetSettingsUI(float posX, float posY)
{
    return std::make_unique<UIBrushSettings>(m_ParentScene, posX, posY);
}

void ToolBrush::UseTool()
{

}
