#pragma once

#include "IScene.hpp"
#include "../OpenGL.hpp"
#include "../scan/Scan.hpp"

#include <vector>
#include <memory>

class BasicScene : public IScene
{
	private:
		std::unique_ptr<UIMenuBar>		m_MenuBar;
		std::unique_ptr<UIToolBar>	    m_ToolBar;
		std::unique_ptr<UIToolSettings> m_ToolSettings;

		std::vector<UIScanImageWindow>  m_ScanWindows;

		Scan m_Scan;

		std::shared_ptr<Texture>	m_AxialTexture;
		std::shared_ptr<Texture>	m_CoronalTexture;
		std::shared_ptr<Texture>	m_SagittalTexture;
		std::shared_ptr<Shader>		m_Shader;

	public:
		BasicScene(const std::string& inputImageFileName);
		virtual ~BasicScene() = default;

		virtual PathsPack RequestPathsPack() override;

		virtual void Input() override;
		virtual void Update() override;
		virtual void Render() override;
		virtual void SetTool() override;
};