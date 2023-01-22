#pragma once

#include "IUIPanel.hpp"
#include "UIScanImageWindow.hpp"

struct PathsPack
{
	std::vector<Path>* axialPaths = nullptr;
	std::vector<Path>* coronalPaths = nullptr;
	std::vector<Path>* sagittalPaths = nullptr;
};

class UIToolSettings : public IUIPanel
{
	private:
		PathsPack m_Paths;

	public:
		UIToolSettings(float posX = 0.0f, float posY = 0.0f);
		virtual ~UIToolSettings() = default;

		void LoadBrushPaths(const PathsPack& paths);

		virtual void Render() override;
};