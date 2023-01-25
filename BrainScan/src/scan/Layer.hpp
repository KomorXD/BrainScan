#include "imgui/imgui.h"
#include "../OpenGL.hpp"

#include <vector>
#include <memory>

struct Point
{
	ImVec2 position{ 0.0f, 0.0f };
	ImVec4 color{ 0.0f, 1.0f, 0.0f, 1.0f };
	float radius = 1.0f;
};

struct Path
{
	std::vector<Point> points;
	bool shoudlDraw = true;
	uint32_t pathID = 0;

	static float s_Color[3];
	static std::unique_ptr<VertexArray>  s_PathVAO;
	static std::unique_ptr<VertexBuffer> s_PathVBO;
	static std::unique_ptr<IndexBuffer>  s_PathIBO;
	static std::unique_ptr<Shader>		 s_PathsShader;

	static void InitializeBuffers();
};

struct Layer 
{
	float* buffer = nullptr;
	std::vector<Path> paths;
};