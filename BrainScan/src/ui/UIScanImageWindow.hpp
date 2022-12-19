#pragma once

#include "IUIPanel.hpp"

#include <memory>

class Texture;

class UIScanImageWindow : public IUIPanel
{
	private:
		int m_ID;
		std::unique_ptr<Texture> m_ScanTexture;

	public:
		UIScanImageWindow(int scanID, float posX, float posY, float height);
		virtual ~UIScanImageWindow() = default;

		void SetScanTexture(std::unique_ptr<Texture>&& text);

		virtual void Render() override;
};