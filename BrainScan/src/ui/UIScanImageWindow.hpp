#pragma once

#include "IUIPanel.hpp"
#include "imgui/imgui.h"
#include "../OpenGL.hpp"

#include <memory>
#include <vector>

struct Point
{
	ImVec2 position{ 0.0f, 0.0f };
	ImVec4 color{ 1.0f, 0.0f, 0.0f, 1.0f };

	float radius = 1.0f;
};

struct Path
{
	std::vector<Point> points;
	bool shoudlDraw = true;
	uint32_t pathID = 0;
};

class UIScanImageWindow : public IUIPanel
{
	private:
		static std::unique_ptr<VertexArray> VAO;
		static std::unique_ptr<IndexBuffer> IBO;

		int m_ID;
		std::shared_ptr<Texture> m_ScanTexture;
		std::shared_ptr<Framebuffer> m_FB;
		std::shared_ptr<Shader> m_Shader;
		std::vector<Path> m_Paths;

		float m_ImageRatio = 1.0f;

		bool m_IsDraggedOver = false;

	public:
		UIScanImageWindow(int scanID, float posX, float posY, float height);
		virtual ~UIScanImageWindow() = default;

		void SetScanTexture(std::shared_ptr<Texture>& text);
		void SetShader(std::shared_ptr<Shader>& shader);
		void SetImageRatio(float ratio);

		inline std::vector<Path>* GetBrushPaths() { return &m_Paths; }

		virtual void Render() override;

		static void InitializeBuffers(uint32_t width, uint32_t height);
};