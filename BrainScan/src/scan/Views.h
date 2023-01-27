#pragma once
#include <vector>
#include <string>
#include "Layer.hpp"

class View
{
private:
	std::vector<Layer> m_Data;
	unsigned int m_Depth = 0;
	unsigned int m_Height = 0;
	unsigned int m_Width = 0;

	uint32_t m_CurrentDepth = 0;

public:
	void InitializeView(unsigned int vectorSize,unsigned int bufferHeight,unsigned int bufferWidth);
	~View();

	float* GetBuffer(const unsigned int& index);

	bool TraverseUp();
	bool TraverseDown();

	void SetCurrentDepth(uint32_t depth);

	inline std::vector<Layer>& GetData() { return m_Data; }
	inline Layer& GetCurrentDepthData() { return m_Data[m_CurrentDepth]; }
	inline unsigned int GetWidth() { return m_Width; }
	inline unsigned int GetHeight() { return m_Height; }
	inline unsigned int GetDepth() { return m_Depth; }
	inline uint32_t GetCurrentDepth() { return m_CurrentDepth; }
};

class Views
{
public:
	View axial;		
	View sagittal;	
	View coronal;	

	void InitializeViews(unsigned int axialWidth, unsigned int axialHeight, unsigned int axialDepth);
};


