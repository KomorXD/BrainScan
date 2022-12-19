#include "Scan.hpp"

#include <assert.h>
#include <typeinfo>
#include <cstdlib>
#include <string>
#include <iostream>

//#include <iostream>
//#include <fstream>

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
		reader.SetFileName(inputImageFileName);
		sitk::Image image = reader.Execute();

		if (image.GetDimension() != Dimension)
		{
			LOG_CRITICAL("Input image is not a {} dimensional image as expected!", Dimension);
			return false;
		}

		sitk::CastImageFilter caster;
		caster.SetOutputPixelType(sitk::sitkInt32);
		m_image = caster.Execute(image);
		
	}
	catch (const std::exception& e)
	{
		LOG_CRITICAL("{}", e.what());
		return false;
	}

	
	m_depth = m_image.GetDepth();
	m_height = m_image.GetHeight();
	m_width = m_image.GetWidth();

	RewriteBuffer();
    return true;
}

void Scan::RewriteBuffer()
{
	//std::ofstream myfile;
	//myfile.open("example.csv");

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
				//myfile << abs(m_image.GetPixelAsInt32(pixelIndex)) << ";";
			}
			//myfile << std::endl;
		}
		//myfile << std::endl;
	}
	//myfile.close();
}
