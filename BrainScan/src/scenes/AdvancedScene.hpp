#pragma once

#include "IScene.hpp"
#include "../OpenGL.hpp"
#include "../scan/Scan.hpp"
#include "../tools/ITool.hpp"

#include <vector>
#include <memory>

class AdvancedScene : public IScene
{
	private:
		std::unique_ptr<UIMenuBar>	    m_MenuBar;
		std::unique_ptr<UIToolBar>	    m_ToolBar;
		std::unique_ptr<UIToolSettings> m_ToolSettings;

		std::vector<UIScanImageWindow> m_ScanWindows;

		std::unique_ptr<ITool> m_CurrentTool;
		Scan m_Scan;

		std::shared_ptr<Texture> m_AxialTexture;
		std::shared_ptr<Texture> m_CoronalTexture;
		std::shared_ptr<Texture> m_SagittalTexture;
		std::shared_ptr<Shader>	m_Shader;

	public:
		AdvancedScene(const std::string& inputImageFileName);
		virtual ~AdvancedScene() = default;

		virtual PathsPack RequestPathsPack() override;

		virtual void Input() override;
		virtual void Update() override;
		virtual void Render() override;
		virtual void SetTool() override;
};