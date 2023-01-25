#pragma once

#include "UIToolSettings.hpp"
#include "UIScanImageWindow.hpp"

struct PathsPack
{
	std::vector<Path>* axialPaths = nullptr;
	std::vector<Path>* coronalPaths = nullptr;
	std::vector<Path>* sagittalPaths = nullptr;
};

class UIBrushSettings : public UIToolSettings
{
	private:
		PathsPack m_Paths;

	public:
		UIBrushSettings(IScene* parentScene, float posX = 0.0f, float posY = 0.0f);
		virtual ~UIBrushSettings() = default;

		virtual void Render() override;

	private:
		void UpdateBrushPaths();
};