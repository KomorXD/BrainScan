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

	float* GetBuffer(const unsigned int& index);

	inline std::vector<float*> GetData() { return m_Data; }
	inline unsigned int GetWidth() { return m_Width; }
	inline unsigned int GetHeight() { return m_Height; }
	inline unsigned int GetDepth() { return m_Depth; }
};

class Views
{
public:
	View axial;		
	View sagittal;	
	View coronal;	

	void InitializeViews(unsigned int axialWidth, unsigned int axialHeight, unsigned int axialDepth);
};


