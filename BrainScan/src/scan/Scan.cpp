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

	sitk::ImageFileReader reader;
	reader.SetFileName(std::string(inputImageFileName));
	sitk::Image image = reader.Execute();

	//
	// First, define the typedefs that correspond to the types of the input
	// image. This requires foreknowlege of the data type of the input image.
	//
	const unsigned int                                 Dimension = 3;
	typedef float                                      InternalPixelType;
	typedef itk::Image< InternalPixelType, Dimension > InternalImageType;

	//
	// We must check the image dimension and the pixel type of the
	// SimpleITK image match the ITK image we will cast to.s
	//
	if (image.GetDimension() != Dimension)
	{
		LOG_CRITICAL("Input image is not a {} dimensional image as expected!", Dimension);
		return false;
	}

	//
	// The read sitk::Image could be any pixel type. Cast the image, to
	// float so we know what type we have.
	//
	sitk::CastImageFilter caster;
	caster.SetOutputPixelType(sitk::sitkInt32);
	m_image = caster.Execute(image);
	
	m_depth = m_image.GetDepth();
	m_height = m_image.GetHeight();
	m_width = m_image.GetWidth();

	PrintBuffer();

	LOG_INFO("GetWidth {}", m_image.GetWidth());
	LOG_INFO("GetHeight {}", m_image.GetHeight());
	LOG_INFO("GetDepth {}", m_image.GetDepth());


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
