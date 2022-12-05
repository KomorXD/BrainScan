#pragma once
#include <string>

class Serializable
{
public:
	virtual bool SaveToFile(std::string FileName) = 0;
	virtual bool LoadFromFile(std::string FileName) = 0;
};