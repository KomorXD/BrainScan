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
	for (int i = 0; i < m_Data.size(); i++)
	{
		delete[] m_Data[i].buffer;
	}
}

float* View::GetBuffer(const unsigned int& index)
{
	if (index >= m_Data.size() || index < 0)
	{
		LOG_CRITICAL("Index {} is out of range", index);
		return nullptr;
	}

	return m_Data.at(index).buffer;
}

bool View::TraverseUp()
{
	if (m_CurrentDepth == 0)
	{
		return false;
	}

	--m_CurrentDepth;

	return true;
}

bool View::TraverseDown()
{
	if (m_CurrentDepth == m_Depth - 1)
	{
		return false;
	}

	++m_CurrentDepth;

	return true;
}

void View::InitializeView(unsigned int vectorSize, unsigned int bufferHeight, unsigned int bufferWidth)
{
	m_Depth = vectorSize;
	m_Height = bufferHeight;
	m_Width = bufferWidth;
	m_CurrentDepth = vectorSize / 2;

	m_Data.resize(vectorSize);       //Resize depth vector
	for (int i = 0; i < vectorSize; i++)
	{
		m_Data[i].buffer = new float[bufferWidth * bufferHeight];  //Allocate memory buffer
	}
}

