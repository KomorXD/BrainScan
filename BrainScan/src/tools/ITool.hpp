#pragma once

class ITool 
{
public:
	virtual ~ITool() = default;

	virtual void UseTool() = 0;
};