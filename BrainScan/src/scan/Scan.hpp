#pragma once
#include <vector>
#include "SimpleITK-2.3/SimpleITK.h"
#include "../include/SimpleITK-2.3/sitkImageOperators.h"
#include "Serializable.hpp"
#include "sequence/ISequence.hpp"
#include "../Core.hpp"

namespace sitk = itk::simple;
class Scan : public Serializable
{
private:
	sitk::Image m_image;
	int m_depth = 0;
	int m_height = 0;
	int m_width = 0;

public:
	virtual bool SaveToFile(std::string FileName) override;
	virtual bool LoadFromFile(std::string inputImageFileName) override;

private:
	void RewriteBuffer();
};