#pragma once
#include <vector>

#include "Serializable.hpp"
#include "sequence/ISequence.hpp"

class Scan : public Serializable
{
private:
	std::vector<ISequence> sequences;

public:
	virtual bool SaveToFile(std::string FileName) override;
	virtual bool LoadFromFile(std::string inputImageFileName) override;
};