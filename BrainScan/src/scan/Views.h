#pragma once
#include <vector>
#include <string>

class View
{
private:
	std::vector<float*> m_Data;
	unsigned int m_Depth = 0;
	unsigned int m_Height = 0;
	unsigned int m_Width = 0;

public:
	void InitializeView(unsigned int vectorSize,unsigned int bufferHeight,unsigned int bufferWidth);
	~View();

	unsigned int GetWidth();
	unsigned int GetHeight();
	unsigned int GetDepth();
	float* GetBuffer(const unsigned int& index);
	void PrintToCSV(const std::string& fileName);
};

class Views
{
public:
	View axial;		
	View sagittal;	
	View coronal;	

	void InitializeViews(unsigned int axialWidth, unsigned int axialHeight, unsigned int axialDepth);
};


