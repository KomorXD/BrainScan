#pragma once

class IScene
{
	public:
		virtual ~IScene() = default;

		virtual void Input()   = 0;
		virtual void Update()  = 0;
		virtual void Render()  = 0;
		virtual void SetTool() = 0;
};