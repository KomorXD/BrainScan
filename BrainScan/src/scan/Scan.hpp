#pragma once
#include <vector>

#include "SimpleITK-2.3/SimpleITK.h"
#include "../include/SimpleITK-2.3/sitkImageOperators.h"
#include "../Core.hpp"
#include "Serializable.hpp"
#include "Views.h"

namespace sitk = itk::simple;
class Scan : public Serializable
{
private:
	sitk::Image m_Image;
	Views m_Views;
	int m_Min;
	int m_Max;

public:
	Scan();
	virtual bool SaveToFile(const std::string & fileName) override;
	virtual bool LoadFromFile(const std::string & inputImageFileName) override;

	inline View* GetAxial() { return &m_Views.axial; }
	inline View* GetCoronal() { return &m_Views.coronal; }
	inline View* GetSagittal() { return &m_Views.sagittal; }

private:
	void CreateViews();
	void PopulateAxialView();
	void CreateSagittalViewBasedOnAxialView();
	void CreateCoronalViewBasedOnAxialView();
	void FindMinMax(View& view);
	float NormalizeValue(int value);
};