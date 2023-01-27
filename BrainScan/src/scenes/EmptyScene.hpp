#pragma once

#include "IScene.hpp"

class EmptyScene : public IScene
{
private:
		std::unique_ptr<Scan> m_Scan;

public:
		EmptyScene();
		virtual ~EmptyScene() = default;
		virtual void Input() override;
		virtual void Update() override;
		virtual void Render() override;
		virtual void SetTool() override;
};