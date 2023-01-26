#include "Layer.hpp"

std::unique_ptr<VertexArray>  Path::s_PathVAO;
std::unique_ptr<VertexBuffer> Path::s_PathVBO;
std::unique_ptr<IndexBuffer>  Path::s_PathIBO;
std::unique_ptr<Shader>		  Path::s_PathsShader;

float Path::s_Color[3]{1.0f, 0.0f, 0.0f};

void Path::InitializeBuffers()
{
	s_PathsShader = std::make_unique<Shader>("res/shaders/PathsShader.vert", "res/shaders/PathsShader.geom", "res/shaders/PathsShader.frag");

	s_PathVAO = std::make_unique<VertexArray>();
	s_PathVBO = std::make_unique<VertexBuffer>(nullptr, 0);
	s_PathIBO = std::make_unique<IndexBuffer>(nullptr, 0);

	VertexBufferLayout layout;

	layout.Push<float>(2);
	layout.Push<float>(3);

	s_PathVAO->AddBuffer(*Path::s_PathVBO, layout);
	s_PathVAO->Unbind();
	s_PathVBO->Unbind();
	s_PathIBO->Unbind();
}