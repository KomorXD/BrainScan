#pragma once

#include "IUIPanel.hpp"
#include "imgui/imgui.h"
#include "../OpenGL.hpp"
#include "../scan/Views.h"

#include <memory>
#include <vector>


class UIScanImageWindow : public IUIPanel
{
	private:
		static std::unique_ptr<VertexArray> s_ScanVAO;
		static std::unique_ptr<IndexBuffer> s_ScanIBO;

		std::string m_Label;
		
		std::shared_ptr<Texture> m_ScanTexture;
		std::shared_ptr<Framebuffer> m_FB;
		std::shared_ptr<Shader> m_Shader;
		
		View* m_View = nullptr;
		int32_t m_Depth = 75;

		bool m_IsDraggedOver = false;

	public:
		static bool s_DrawingEnabled;

		UIScanImageWindow(const std::string& label, float posX, float posY, float height);
		virtual ~UIScanImageWindow() = default;

		void SetScanTexture(std::shared_ptr<Texture>& text);
		void SetShader(std::shared_ptr<Shader>& shader);
		void SetImageRatio(float ratio);
		void SetView(View* view);
		inline std::vector<Path>* GetBrushPaths() { return m_View ? &m_View->GetData().at(m_Depth).paths : nullptr; }

		virtual void Render() override;

		static void InitializeBuffers(uint32_t width, uint32_t height);

	private:
		void CheckForDrawing();
		void RenderScanAndBrushes();
};