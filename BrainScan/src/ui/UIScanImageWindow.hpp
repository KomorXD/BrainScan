#pragma once

#include "IUIPanel.hpp"

class UIScanImageWindow : public IUIPanel
{
	private:
		int m_ID;

	public:
		UIScanImageWindow(int scanID, float posX, float posY, float height);
		virtual ~UIScanImageWindow() = default;

		virtual void Render() override;
};