#include <iostream>
#include <fstream>

#include "Views.h"
#include "../Core.hpp"

void Views::InitializeViews(unsigned int axialWidth, unsigned int axialHeight, unsigned int axialDepth)
{
	axial.InitializeView(axialDepth, axialHeight, axialWidth);
	sagittal.InitializeView(axialWidth, axialDepth, axialHeight );
	coronal.InitializeView(axialHeight, axialDepth, axialWidth);
}

View::~View()
{
	for (size_t i = 0; i < m_Depth; i++)
	{
//		delete[] m_Data[i];
	}
}

float* View::GetBuffer(const unsigned int& index)
{
	if (index >= m_Data.size() || index < 0)
	{
		LOG_CRITICAL("Index {} is out of range", index);
		return nullptr;
	}

	return m_Data.at(index);
}

unsigned int View::GetWidth()
{
	return m_Width;
}

unsigned int View::GetHeight()
{
	return m_Height;
}

unsigned int View::GetDepth()
{
	return m_Depth;
}

void View::PrintToCSV(const std::string& fileName)
{
	std::ofstream myfile;
	myfile.open(fileName + ".csv");
	for (size_t d = 0; d < m_Depth; d++)
	{
		for (size_t h = 0; h < m_Height; h++)
		{
			for (size_t w = 0; w < m_Width; w++)
			{
				myfile << GetBuffer(d)[w + (m_Width * h)]<<";";
			}
			myfile << std::endl;
		}
		myfile << std::endl;
	}
	
	myfile.close();
}

void View::InitializeView(unsigned int vectorSize, unsigned int bufferHeight, unsigned int bufferWidth)
{
	m_Depth = vectorSize;
	m_Height = bufferHeight;
	m_Width = bufferWidth;

	m_Data.resize(vectorSize);       //Resize depth vector
	for (int i = 0; i < vectorSize; i++)
	{
		m_Data[i] = new float[bufferWidth * bufferHeight];  //Allocate memory buffer
	}
}

