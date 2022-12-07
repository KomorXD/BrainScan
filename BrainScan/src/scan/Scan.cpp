#include "Scan.hpp"
#include "SimpleITK-2.3/SimpleITK.h"
#include <assert.h>
#include <typeinfo>
#include <cstdlib>
#include <string>
#include "../Core.hpp"


bool Scan::SaveToFile(std::string inputImageFileName)
{

    return false;
}

bool Scan::LoadFromFile(std::string inputImageFileName)
{
	itk::simple::ImageFileReader reader;
	reader.SetImageIO("");
	reader.SetFileName(inputImageFileName);

	itk::simple::Image image;
	image = reader.Execute();

	LOG_INFO("GetWidth {}",image.GetWidth());
	LOG_INFO("GetHeight {}",image.GetHeight());
	LOG_INFO("GetDepth {}",image.GetDepth());

    return false;
}
