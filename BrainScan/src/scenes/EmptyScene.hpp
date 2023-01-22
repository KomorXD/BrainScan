#pragma once

#include "IScene.hpp"
#include "../ui/UIPanels.hpp"
#include "../OpenGL.hpp"
#include "../scan/Scan.hpp"

#include <vector>
#include <memory>

class EmptyScene : public IScene
{
	private:
		std::unique_ptr<UIMenuBar> m_MenuBar;
		std::unique_ptr<UIToolBar> m_ToolBar;

	public:
		EmptyScene();
		virtual ~EmptyScene() = default;

		virtual void Input() override;
		virtual void Update() override;
		virtual void Render() override;
		virtual void SetTool() override;
};