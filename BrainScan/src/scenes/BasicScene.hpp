#pragma once

#include "IScene.hpp"

class BasicScene : public IScene
{
public:
	virtual ~BasicScene() = default;

	virtual void Input() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void SetTool() override;
};