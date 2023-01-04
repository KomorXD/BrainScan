#pragma once
#include <string>

class Serializable
{
public:
	virtual bool SaveToFile(const std::string & fileName) = 0;
	virtual bool LoadFromFile(const std::string & fileName) = 0;
};