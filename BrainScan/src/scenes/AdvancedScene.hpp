#pragma once

#include "IScene.hpp"

class AdvancedScene : public IScanScene
{
	public:
		AdvancedScene(std::unique_ptr<Scan>&& scan);
		virtual ~AdvancedScene() = default;

	private:
		virtual void PopulateMenu() override;
		virtual void PopulateToolBar() override;
};