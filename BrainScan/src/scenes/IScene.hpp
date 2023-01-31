#pragma once

#include "../ui/UIPanels.hpp"
#include "../OpenGL.hpp"
#include "../scan/Scan.hpp"

#include <vector>
#include <memory>

class ITool;

class IScene
{
	protected:
		std::unique_ptr<UIMenuBar> m_MenuBar;
		std::unique_ptr<UIToolBar> m_ToolBar;
		std::unique_ptr<Scan>      m_Scan;
		bool m_ShouldShowModal = false;
		std::string m_InputImageFileName;
	public:
		IScene();
		virtual ~IScene() = default;		
		virtual void Input()   = 0;
		virtual void Update()  = 0;
		virtual void Render()  = 0;
		virtual void SetTool() = 0;
		
	protected:
		void ShowChooseFileDialog();
		void ShowChooseFileModeModal();

};

class IScanScene : public IScene
{
	protected:
		std::unique_ptr<UIToolSettings>    m_ToolSettings;

		std::unique_ptr<UIScanImageWindow> m_AxialWindow;
		std::unique_ptr<UIScanImageWindow> m_CoronalWindow;
		std::unique_ptr<UIScanImageWindow> m_SagittalWindow;
		std::unique_ptr<UIScanImageWindow> m_NullWindow; // leave for now ig

		std::unique_ptr<ITool> m_CurrentTool;

		std::shared_ptr<Shader>	m_Shader;
		
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