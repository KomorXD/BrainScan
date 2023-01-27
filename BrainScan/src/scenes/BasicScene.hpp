#pragma once

#include "IScene.hpp"

class BasicScene : public IScanScene
{
	public:
		BasicScene(std::unique_ptr<Scan>&& scan);
		virtual ~BasicScene() = default;

	protected:
		virtual void PopulateMenu() override;
		virtual void PopulateToolBar() override;
};