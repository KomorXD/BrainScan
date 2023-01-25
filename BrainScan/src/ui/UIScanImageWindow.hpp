#pragma once

#include "IUIPanel.hpp"
#include "imgui/imgui.h"
#include "../OpenGL.hpp"

#include <memory>
#include <vector>

struct Point
{
	ImVec2 position{ 0.0f, 0.0f };
	ImVec4 color{ 0.0f, 1.0f, 0.0f, 1.0f };

	float radius = 1.0f;
};

struct Path
{
	std::vector<Point> points;
	bool shoudlDraw = true;
	uint32_t pathID = 0;

	static std::unique_ptr<VertexArray>  s_PathVAO;
	static std::unique_ptr<VertexBuffer> s_PathVBO;
	static std::unique_ptr<IndexBuffer>  s_PathIBO;
	static std::unique_ptr<Shader>		 s_PathsShader;
	
	// Temporary before we have a proper tool class and tool settings subclass
	static float s_Color[3];

	static void InitializeBuffers();
};

class UIScanImageWindow : public IUIPanel
{
	private:
		static std::unique_ptr<VertexArray> s_ScanVAO;
		static std::unique_ptr<IndexBuffer> s_ScanIBO;

		std::string m_Label;
		
		std::shared_ptr<Texture> m_ScanTexture;
		std::shared_ptr<Framebuffer> m_FB;
		std::shared_ptr<Shader> m_Shader;
		std::vector<Path> m_Paths;

		bool m_IsDraggedOver = false;

	public:
		UIScanImageWindow(const std::string& label, float posX, float posY, float height);
		virtual ~UIScanImageWindow() = default;

		void SetScanTexture(std::shared_ptr<Texture>& text);
		void SetShader(std::shared_ptr<Shader>& shader);
		void SetImageRatio(float ratio);

		inline std::vector<Path>* GetBrushPaths() { return &m_Paths; }

		virtual void Render() override;

		static void InitializeBuffers(uint32_t width, uint32_t height);

	private:
		void CheckForDrawing();
		void RenderScanAndBrushes();
};