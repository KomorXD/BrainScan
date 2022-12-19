#include "Scan.hpp"

#include <assert.h>
#include <typeinfo>
#include <cstdlib>
#include <string>
#include <iostream>

namespace sitk = itk::simple;
bool Scan::SaveToFile(std::string inputImageFileName)
{

    return false;
}

bool Scan::LoadFromFile(std::string inputImageFileName)
{
	const unsigned int                                 Dimension = 3;
	typedef float                                      InternalPixelType;
	typedef itk::Image< InternalPixelType, Dimension > InternalImageType;

	try
	{
		sitk::ImageFileReader reader;
		reader.SetFileName(std::string(inputImageFileName));
		sitk::Image image = reader.Execute();

		if (image.GetDimension() != Dimension)
		{
			LOG_CRITICAL("Input image is not a {} dimensional image as expected!", Dimension);
			return false;
		}

		sitk::CastImageFilter caster;
		caster.SetOutputPixelType(sitk::sitkLabelUInt8);
		m_image = caster.Execute(image);
		sitk::WriteImage(m_image, "blob.png");
	}
	catch (const std::exception& e)
	{
		LOG_ERROR("{}", e.what());
	}

	
	m_depth = m_image.GetDepth();
	m_height = m_image.GetHeight();
	m_width = m_image.GetWidth();

	//PrintBuffer();
	//sitk::WriteImage(m_image, "blob.png");
    return false;
}

void Scan::PrintBuffer()
{
	std::vector<unsigned int> pixelIndex;
	for (int d = 0; d < m_depth; d++)
	{
		for (int h = 0; h < m_height; h++)
		{
			for (int w = 0; w < m_width; w++)
			{
				pixelIndex = { { static_cast<unsigned int>(w),
								 static_cast<unsigned int>(h),
								 static_cast<unsigned int>(d)
								} };
				//myfile << m_image.GetPixelAsInt32(pixelIndex) << ";";
			}
		}
	}
}
