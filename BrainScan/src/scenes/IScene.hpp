#pragma once

#include "../ui/UIPanels.hpp"
#include "../OpenGL.hpp"
#include "../scan/Scan.hpp"
#include "../tools/ITool.hpp"

#include <vector>
#include <memory>

class IScene
{
	protected:
		std::unique_ptr<UIMenuBar> m_MenuBar;
		std::unique_ptr<UIToolBar> m_ToolBar;

	public:
		virtual ~IScene() = default;		
		virtual void Input()   = 0;
		virtual void Update()  = 0;
		virtual void Render()  = 0;
		virtual void SetTool() = 0;
};

class IScanScene : public IScene
{
	protected:
		std::unique_ptr<UIToolSettings> m_ToolSettings;
		std::vector<UIScanImageWindow>  m_ScanWindows;

		std::unique_ptr <Scan> m_Scan;
		std::shared_ptr<Texture>	m_AxialTexture;
		std::shared_ptr<Texture>	m_CoronalTexture;
		std::shared_ptr<Texture>	m_SagittalTexture;
		std::shared_ptr<Shader>		m_Shader;

	private:
		float m_ScanPanelHeight = 0;

	public:
		IScanScene(std::unique_ptr<Scan>&& scan);
		virtual ~IScanScene() = default;

		PathsPack RequestPathsPack();
		virtual void Input() override;
		virtual void Update() override;
		virtual void SetTool() override;

	protected:
		void OnScroll(double offset);
		virtual void PopulateMenu() = 0;
		virtual void PopulateToolBar() = 0;
		virtual void Render() override;

	private:
		void CreateScrollHandler();
		void CreateImageTexture(std::unique_ptr<Scan>&& scan);
		void CreateUIElements();
};