#pragma once

#include "IScene.hpp"

class AdvancedScene : public IScene
{
public:
	virtual ~AdvancedScene() = default;

	virtual void Input() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void SetTool() override;
};