#include "Scan.hpp"

#include <assert.h>
#include <typeinfo>
#include <cstdlib>
#include <string>
#include <iostream>
#include <math.h>

//#include <iostream>
//#include <fstream>

namespace sitk = itk::simple;
bool Scan::SaveToFile(const std::string & inputImageFileName)
{

    return false;
}

bool Scan::LoadFromFile(const std::string & inputImageFileName)
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
		m_Image = caster.Execute(image);
		
	}
	catch (const std::exception& e)
	{
		LOG_CRITICAL("{}", e.what());
		return false;
	}

	
	m_Depth = m_Image.GetDepth();
	m_Height = m_Image.GetHeight();
	m_Width = m_Image.GetWidth();

	m_Views.InitializeViews(m_Width, m_Height, m_Depth);
	FindMinMaxValues();
	CreateViews();

    return true;
}

void Scan::CreateAxialView()
{
	std::vector<unsigned int> pixelIndex;
	for (int d = 0; d < m_Depth; d++)
	{
		for (int h = 0; h < m_Height; h++)
		{
			for (int w = 0; w < m_Width; w++)
			{				
				pixelIndex = { { static_cast<unsigned int>(w),
								 static_cast<unsigned int>(h),
								 static_cast<unsigned int>(d)
								} };
				m_Views.axial.data.at(d)[w + (m_Width * h)] = NormalizeValue(m_Image.GetPixelAsInt32(pixelIndex));
			}
		}
	}
}

void Scan::CreateSagittalView()
{
	std::vector<unsigned int> pixelIndex;
	for (int h = 0; h < m_Height; h++)
	{
		for (int d = 0; d < m_Depth; d++)
		{
			for (int w = 0; w < m_Width; w++)
			{
				pixelIndex = { { static_cast<unsigned int>(w),
								 static_cast<unsigned int>(d),
								 static_cast<unsigned int>(h)
								} };
				//m_Views.sagittal.data.at(h)[w + (m_Width * d)] = NormalizeValue(m_Image.GetPixelAsInt32(pixelIndex));
			}
		}
	}
}

void Scan::CreateCoronalView()
{
	std::vector<unsigned int> pixelIndex;
	for (int w = 0; w < m_Width; w++)
	{
		for (int h = 0; h < m_Height; h++)
		{
			for (int d = 0; d < m_Depth; d++)
			{
				pixelIndex = { { static_cast<unsigned int>(d),
								 static_cast<unsigned int>(h),
								 static_cast<unsigned int>(w)
								} };
				m_Views.axial.data.at(w)[d + (m_Depth * h)] = NormalizeValue(m_Image.GetPixelAsInt32(pixelIndex));
			}
		}
	}
}

void Scan::CreateViews()
{
	CreateAxialView();
	CreateCoronalView();
	CreateSagittalView();
}

void Scan::FindMinMaxValues()
{
	std::vector<unsigned int> pixelIndex;
	for (int d = 0; d < m_Depth; d++)
	{
		for (int h = 0; h < m_Height; h++)
		{
			for (int w = 0; w < m_Width; w++)
			{
				pixelIndex = { { static_cast<unsigned int>(w),
								 static_cast<unsigned int>(h),
								 static_cast<unsigned int>(d)
								} };
				int pixelValue = m_Image.GetPixelAsInt32(pixelIndex);
				if (pixelValue < m_Min)
				{
					m_Min = pixelValue;
				}
				else if (pixelValue > m_Max)
				{
					m_Max = pixelValue;
				}
			}
		}
	}
}

float Scan::NormalizeValue(int value)
{
	return (float)(value - m_Min) / (m_Max - m_Min);
}

Views::~Views()
{
	for (float* buffer : axial.data)
	{
		delete[] buffer;
	}
	for (float* buffer : sagittal.data)
	{
		delete[] buffer;
	}
	for (float* buffer : coronal.data)
	{
		delete[] buffer;
	}
}

void Views::InitializeViews(unsigned int width, unsigned int height, unsigned int depth)
{
	InitializeAxial(depth, width, height);
	InitializeSagittal(height, depth, width);
	InitializeCoronal(width, height, depth);
}

void Views::InitializeCoronal(unsigned int width, unsigned int height, unsigned int depth)
{
	coronal.data.resize(width);
	for (int i = 0; i < width; i++)
	{
		coronal.data[i] = new float[depth * height];
	}
}

void Views::InitializeSagittal(unsigned int height, unsigned int depth, unsigned int width)
{
	sagittal.data.resize(height);
	for (int i = 0; i < height; i++)
	{
		sagittal.data[i] = new float[width * depth];
	}
}

void Views::InitializeAxial(unsigned int depth, unsigned int width, unsigned int height)
{
	axial.data.resize(depth);
	for (int i = 0; i < depth; i++)
	{
		axial.data[i] = new float[width * height];
	}
}
