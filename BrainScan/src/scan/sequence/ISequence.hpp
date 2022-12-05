#pragma once

#include "Layer.hpp"

class ISequence
{
public:
	virtual Layer* GetCurrentLayer() = 0;
};