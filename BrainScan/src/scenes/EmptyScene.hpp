#pragma once

#include "IScene.hpp"
#include "../ui/IUIPanel.hpp"

#include <vector>
#include <memory>

class EmptyScene : public IScene
{
	private:
		std::vector<std::unique_ptr<IUIPanel>> m_Panels;

	public:
		EmptyScene();
		virtual ~EmptyScene() = default;

		virtual void Input() override;
		virtual void Update() override;
		virtual void Render() override;
		virtual void SetTool() override;
};