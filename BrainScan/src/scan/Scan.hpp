#pragma once
#include <vector>
#include "SimpleITK-2.3/SimpleITK.h"
#include "../include/SimpleITK-2.3/sitkImageOperators.h"
#include "Serializable.hpp"
#include "sequence/ISequence.hpp"
#include "../Core.hpp"

struct View
{
	std::vector<float*> data;
};

class Views
{
public:
	View axial;				//vector size = depth,		float* = height * width
	View sagittal;			//vector size = height,		float* = depth  * width
	View coronal;			//vector size = width,		float* = height * depth

	~Views();

	void InitializeViews(unsigned int width, unsigned int height, unsigned int depth);

private:
	void InitializeCoronal(unsigned int width, unsigned int height, unsigned int depth);
	void InitializeSagittal(unsigned int height, unsigned int depth, unsigned int width);
	void InitializeAxial(unsigned int depth, unsigned int width, unsigned int height);
};


namespace sitk = itk::simple;
class Scan : public Serializable
{
private:
	sitk::Image m_Image;
	int m_Depth = 0;
	int m_Height = 0;
	int m_Width = 0;

	int m_Min = INT32_MAX;
	int m_Max = INT32_MIN;

	Views m_Views;

public:
	virtual bool SaveToFile(const std::string & fileName) override;
	virtual bool LoadFromFile(const std::string & inputImageFileName) override;

private:
	void CreateAxialView();
	void CreateSagittalView();
	void CreateCoronalView();
	void CreateViews();

	void FindMinMaxValues();
	float NormalizeValue(int value);
};