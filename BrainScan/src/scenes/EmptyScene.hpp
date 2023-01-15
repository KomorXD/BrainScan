#pragma once

#include "IScene.hpp"
#include "../ui/IUIPanel.hpp"
#include "../OpenGL.hpp"
#include "../scan/Scan.hpp"

#include <vector>
#include <memory>

class EmptyScene : public IScene
{
	private:
		std::vector<std::unique_ptr<IUIPanel>> m_Panels;
		//Scan m_Scan;

		/*
			TEMPORARY
		*/
		//Framebuffer m_FB;
		//std::shared_ptr<Texture> m_AxialTexture;
		//std::shared_ptr<Texture> m_CoronalTexture;
		//std::shared_ptr<Texture> m_SagittalTexture;
		//std::unique_ptr<VertexArray> m_VAO;
		//std::unique_ptr<IndexBuffer> m_IBO;
		//std::unique_ptr<Shader> m_Shader;

	public:
		EmptyScene();
		virtual ~EmptyScene() = default;

		virtual void Input() override;
		virtual void Update() override;
		virtual void Render() override;
		virtual void SetTool() override;
};