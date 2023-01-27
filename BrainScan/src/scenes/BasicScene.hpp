#pragma once

#include "IScene.hpp"

class BasicScene : public IScanScene
{
	private:

		std::unique_ptr<ITool> m_CurrentTool;

	public:
		BasicScene(std::unique_ptr<Scan>&& scan);
		virtual ~BasicScene() = default;


		virtual void Input() override;
		virtual void Update() override;
		virtual void SetTool() override;
};