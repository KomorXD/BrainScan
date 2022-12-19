#include "Scan.hpp"

#include <assert.h>
#include <typeinfo>
#include <cstdlib>
#include <string>

namespace sitk = itk::simple;
Scan::Scan()
{
	m_Min = INT32_MAX;
	m_Max = INT32_MIN;
}
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

	unsigned int axialWidth = m_Image.GetWidth();
	unsigned int axialHeight = m_Image.GetHeight();
	unsigned int axialDepth = m_Image.GetDepth();
	m_Views.InitializeViews(axialWidth, axialHeight, axialDepth);
	FindMinMax(m_Views.axial);
	CreateViews();

    return true;
}

void Scan::CreateViews()
{
	PopulateAxialView();
	CreateSagittalViewBasedOnAxialView();
	CreateCoronalViewBasedOnAxialView();
}

void Scan::PopulateAxialView()
{
	std::vector<unsigned int> pixelIndex;
	unsigned int rowLength = m_Views.axial.GetWidth();

	for (int d = 0; d < m_Views.axial.GetDepth(); d++)
	{
		for (int h = 0; h < m_Views.axial.GetHeight(); h++)
		{
			for (int w = 0; w < m_Views.axial.GetWidth(); w++)
			{
				pixelIndex = { { static_cast<unsigned int>(w),
								 static_cast<unsigned int>(h),
								 static_cast<unsigned int>(d)
								} };

				float value = NormalizeValue(m_Image.GetPixelAsInt32(pixelIndex));
				m_Views.axial.GetBuffer(d)[w + (rowLength * h)] = value;
			}
		}
	}
}

void Scan::CreateSagittalViewBasedOnAxialView()
{
	unsigned int axialRowLength = m_Views.axial.GetWidth();

	for (int w = 0; w < m_Views.axial.GetWidth(); w++)
	{	
		for (int d = 0; d < m_Views.axial.GetDepth(); d++)
		{		
			for (int h = 0; h < m_Views.axial.GetHeight(); h++)
			{
				float value = m_Views.axial.GetBuffer(d)[w + (axialRowLength * h)];
				m_Views.sagittal.GetBuffer(w)[h + (m_Views.sagittal.GetWidth() * d)] = value;
			}
		}
	}
}

void Scan::CreateCoronalViewBasedOnAxialView()
{
	unsigned int axialRowLength = m_Views.axial.GetWidth();
	for (int h = 0; h < m_Views.axial.GetHeight(); h++)
	{
		for (int d = 0; d < m_Views.axial.GetDepth(); d++)
		{
			for (int w = 0; w < m_Views.axial.GetWidth(); w++)
			{
				float value = m_Views.axial.GetBuffer(d)[w + (axialRowLength * h)];
				m_Views.coronal.GetBuffer(h)[w + (m_Views.coronal.GetWidth() * d)] = value;
			}
		}
	}
}

void Scan::FindMinMax(View& view)
{
	std::vector<unsigned int> pixelIndex;
	for (int d = 0; d < view.GetDepth(); d++)
	{
		for (int h = 0; h < view.GetHeight(); h++)
		{
			for (int w = 0; w < view.GetWidth(); w++)
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

