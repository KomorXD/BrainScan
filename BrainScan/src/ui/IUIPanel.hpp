#pragma once

class IUIPanel
{
	protected:
		float m_PosX;
		float m_PosY;
		float m_Width  = 0.0f;
		float m_Height = 0.0f;

	public:
		IUIPanel(float posX = 0.0f, float posY = 0.0f);
		virtual ~IUIPanel() = default;

		virtual void Render() = 0;

		virtual inline float GetWidth()  const { return m_Width;  }
		virtual inline float GetHeight() const { return m_Height; }

		virtual inline float GetPosX() const { return m_PosX; }
		virtual inline float GetPosY() const { return m_PosY; }

	protected:
		virtual void Update();
};