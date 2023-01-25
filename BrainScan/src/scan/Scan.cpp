#include "Scan.hpp"

#include <future>
#include <assert.h>
#include <typeinfo>
#include <cstdlib>
#include <string>

namespace sitk = itk::simple;

bool Scan::SaveToFile(const std::string & inputImageFileName)
{
    return false;
}

void Scan::CreateMockData()
{
	CreateMock(128);
}

void Scan::CreateMockData(int size)
{
	CreateMock(size);
}

void Scan::CreateMock(int size)
{
	FUNC_PROFILE();
	sitk::PixelIDValueEnum pixelType = sitk::sitkInt32;

	if (size <= 128)
		size = 128;

	std::vector<unsigned int> imageSize(3, size);

	std::vector<double> faceSize(3, 64.0);
	std::vector<double> faceCenter(3, 64.0);;
	sitk::Image face = sitk::GaussianSource(pixelType, imageSize, faceSize, faceCenter);

	std::vector<double> eyeSize(3, 5.0);
	std::vector<double> eye1Center(3, 48.0);
	std::vector<double> eye2Center = { 80.0, 48.0 , 52.0 };
	sitk::Image eye1 = sitk::GaussianSource(pixelType, imageSize, eyeSize, eye1Center, 150);
	sitk::Image eye2 = sitk::GaussianSource(pixelType, imageSize, eyeSize, eye2Center, 150);

	face = face - eye1 - eye2;
	face = sitk::BinaryThreshold(face, 200, 255, 255);

	std::vector<double> mouthRadii = { 30.0, 20.0 , 25.0 };
	std::vector<double> mouthCenter = { 64.0, 76.0 , 70.0 };
	sitk::Image mouth = 255 - sitk::BinaryThreshold(
		sitk::GaussianSource(pixelType, imageSize, mouthRadii, mouthCenter),
		200, 255, 255);

	std::vector<unsigned int> mouthSize = { 64, 18 , 20 };
	std::vector<int> mouthLoc = { 32, 76 , 14 };
	face = sitk::Paste(face, mouth, mouthSize, mouthLoc, mouthLoc);

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

static std::atomic<int> s_Min = INT32_MAX;
static std::atomic<int> s_Max = INT32_MIN;

static void FindMinMaxAtDepth(sitk::Image* image, View* axial, unsigned int depth)
{
	std::vector<unsigned int> pixelIndex;
	int pixelValue = 0;
	for (int h = 0; h < axial->GetHeight(); h++)
	{
		for (int w = 0; w < axial->GetWidth(); w++)
		{
			pixelIndex = { { static_cast<unsigned int>(w),
								static_cast<unsigned int>(h),
								static_cast<unsigned int>(depth)
							} };
			pixelValue = image->GetPixelAsInt32(pixelIndex);

			if (s_Min > pixelValue)
			{
				s_Min = pixelValue;
			}

			if (s_Max < pixelValue) 
			{
				s_Max = pixelValue;
			}
		}
	}
}

void Scan::FindMinMax(View& view)
{
	FUNC_PROFILE();
	std::vector<std::future<void>> futures;
	futures.reserve(m_Views.axial.GetDepth());
	for (int d = 0; d < view.GetDepth(); d++)
	{
		futures.push_back(std::async(std::launch::async, FindMinMaxAtDepth, &m_Image, &m_Views.axial, d));
	}
}

static float NormalizeValue(int value)
{
	return (float)(value - s_Min) / (s_Max - s_Min);
}

static void SetPixelAtDepth(sitk::Image* image, View* axial, unsigned int depth)
{
	std::vector<unsigned int> pixelIndex;
	unsigned int rowLength = axial->GetWidth();

	for (int h = 0; h < axial->GetHeight(); h++)
	{
		for (int w = 0; w < axial->GetWidth(); w++)
		{
			pixelIndex = { { static_cast<unsigned int>(w),
							 static_cast<unsigned int>(h),
							 static_cast<unsigned int>(depth)
							} };

			float value = NormalizeValue(image->GetPixelAsInt32(pixelIndex));
			axial->GetBuffer(depth)[w + (rowLength * h)] = value;
		}
	}
}

void Scan::PopulateAxialView()
{
	FUNC_PROFILE();
	std::vector<std::future<void>> futures;	
	futures.reserve(m_Views.axial.GetDepth());

	for (int d = 0; d < m_Views.axial.GetDepth(); d++)
	{
		futures.push_back(std::async(std::launch::async, SetPixelAtDepth, &m_Image, &m_Views.axial, d));
	}
}

void Scan::CreateSagittalViewBasedOnAxialView()
{
	FUNC_PROFILE();
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
	FUNC_PROFILE();
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


