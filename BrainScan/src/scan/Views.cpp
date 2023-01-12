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

