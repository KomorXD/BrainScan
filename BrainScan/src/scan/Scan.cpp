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

void Scan::CreateMockData()
{
	sitk::PixelIDValueEnum pixelType = sitk::sitkInt32;
	std::vector<unsigned int> imageSize(3, 128);

	// Create an image
	//sitk::Image image(imageSize, pixelType);
	
	// Create a face image
	std::vector<double> faceSize(3, 64.0);
	std::vector<double> faceCenter(3, 64.0);;
	sitk::Image face = sitk::GaussianSource(pixelType, imageSize, faceSize, faceCenter);

	// Create eye images
	std::vector<double> eyeSize(3, 5.0);
	std::vector<double> eye1Center(3, 48.0);
	std::vector<double> eye2Center = { 80.0, 48.0 , 52.0 };
	sitk::Image eye1 = sitk::GaussianSource(pixelType, imageSize, eyeSize, eye1Center, 150);
	sitk::Image eye2 = sitk::GaussianSource(pixelType, imageSize, eyeSize, eye2Center, 150);

	// Apply the eyes to the face
	face = face - eye1 - eye2;
	face = sitk::BinaryThreshold(face, 200, 255, 255);

	// Create the mouth
	std::vector<double> mouthRadii = { 30.0, 20.0 , 25.0 };
	std::vector<double> mouthCenter = { 64.0, 76.0 , 70.0 };
	sitk::Image mouth = 255 - sitk::BinaryThreshold(
		sitk::GaussianSource(pixelType, imageSize, mouthRadii, mouthCenter),
		200, 255, 255);

	// Paste the mouth onto the face
	std::vector<unsigned int> mouthSize = { 64, 18 , 20 };
	std::vector<int> mouthLoc = { 32, 76 , 14 };
	face = sitk::Paste(face, mouth, mouthSize, mouthLoc, mouthLoc);

	// Apply the face to the original image


	sitk::CastImageFilter caster;
	caster.SetOutputPixelType(sitk::sitkInt32);
	m_Image = caster.Execute(face);

	ConvertToInternalViews();
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

	ConvertToInternalViews();

    return true;
}

void Scan::ConvertToInternalViews()
{
	unsigned int axialWidth = m_Image.GetWidth();
	unsigned int axialHeight = m_Image.GetHeight();
	unsigned int axialDepth = m_Image.GetDepth();
	m_Views.InitializeViews(axialWidth, axialHeight, axialDepth);
	FindMinMax(m_Views.axial);
	CreateViews();
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

