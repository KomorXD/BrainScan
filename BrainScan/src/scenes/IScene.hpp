#pragma once

#include "../ui/UIPanels.hpp"

class IScene
{
	public:
		virtual ~IScene() = default;

		virtual PathsPack RequestPathsPack() = 0;

		virtual void Input()   = 0;
		virtual void Update()  = 0;
		virtual void Render()  = 0;
		virtual void SetTool() = 0;
};