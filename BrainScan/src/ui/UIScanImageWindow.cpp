#include "UIScanImageWindow.hpp"
#include "imgui/imgui.h"
#include "../Core.hpp"
#include "../OpenGL.hpp"

#include <format>

#define BUFFER_OFFSET(offset) (static_cast<uint8_t*>(0) + (offset))

std::unique_ptr<VertexArray>  Path::s_PathVAO;
std::unique_ptr<VertexBuffer> Path::s_PathVBO;
std::unique_ptr<IndexBuffer>  Path::s_PathIBO;
std::unique_ptr<Shader>		  Path::s_PathsShader;

float Path::s_Color[3];

std::unique_ptr<VertexArray> UIScanImageWindow::s_ScanVAO;
std::unique_ptr<IndexBuffer> UIScanImageWindow::s_ScanIBO;

void Path::InitializeBuffers()
{
	s_PathsShader = std::make_unique<Shader>("res/shaders/PathsShader.vert", "res/shaders/PathsShader.frag");

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

UIScanImageWindow::UIScanImageWindow(int scanID, float posX, float posY, float height)
	: IUIPanel(posX, posY), m_ID(scanID)
{
	m_FB = std::make_shared<Framebuffer>();
	m_FB->UnbindBuffer();

	ImVec2 windowSize = ImGui::GetMainViewport()->Size;

	m_Width = windowSize.x / 3.0f;
	m_Height = height;

	LOG_INFO("Initialized scan panel panel #{}", scanID);

	m_FB->BindBuffer();
	m_FB->AttachTexture((uint32_t)m_Width * 3, (uint32_t)m_Height * 2);
	m_FB->UnbindBuffer();
}

void UIScanImageWindow::SetScanTexture(std::shared_ptr<Texture>& text)
{
	m_ScanTexture = text;
}

void UIScanImageWindow::SetShader(std::shared_ptr<Shader>& shader)
{
	m_Shader = shader;
}

void UIScanImageWindow::SetImageRatio(float ratio)
{
	// m_ImageRatio = ratio;
}

static float dist(Point lhs, ImVec2 rhs)
{
	return std::sqrtf(std::powf(lhs.position.x - rhs.x, 2.0f) + std::powf(lhs.position.y - rhs.y, 2.0f));
}

void UIScanImageWindow::Render()
{
	ImGui::SetNextWindowSize(ImVec2(m_Width, m_Height));
	ImGui::SetNextWindowPos(ImVec2(m_PosX, m_PosY));

	ImGui::Begin(std::format("Scan image axis #{}", m_ID).c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	Update();

	ImGui::BeginChild(std::format("scan#{}", m_ID).c_str(), ImVec2(m_Width - 16.0f, m_Height - ImGui::GetFontSize() * 1.7f - 8.0f), true);

	CheckForDrawing();

	if (m_ScanTexture)
	{
		RenderScanAndBrushes();
	}

	ImGui::EndChild();

	ImGui::End();
}

void UIScanImageWindow::InitializeBuffers(uint32_t width, uint32_t height)
{
	std::array<float, 16> vertices =
	{
		// positions    // texture coords
		 1.0f,  1.0f,   1.0f, 1.0f,
		 1.0f, -1.0f,   1.0f, 0.0f,
		-1.0f, -1.0f,   0.0f, 0.0f,
		-1.0f,  1.0f,   0.0f, 1.0f
	};

	std::array<uint32_t, 6> indices =
	{
		0, 1, 2,
		2, 3, 0
	};

	s_ScanVAO = std::make_unique<VertexArray>();
	VertexBuffer vbo(vertices.data(), 16 * sizeof(float));
	s_ScanIBO = std::make_unique<IndexBuffer>(indices.data(), 6);

	VertexBufferLayout layout;

	layout.Push<float>(2);
	layout.Push<float>(2);

	s_ScanVAO->AddBuffer(vbo, layout);
	s_ScanVAO->Unbind();
	vbo.Unbind();
	s_ScanIBO->Unbind();
}

void UIScanImageWindow::CheckForDrawing()
{
	bool isDraggedNow = ImGui::IsWindowHovered() && ImGui::GetIO().MouseDown[0];

	if (!m_IsDraggedOver && isDraggedNow)
	{
		m_IsDraggedOver = true;

		m_Paths.emplace_back();
		m_Paths.back().pathID = m_Paths.size() == 1 ? 1 : (++m_Paths.rbegin())->pathID + 1;
	}
	else if (m_IsDraggedOver && !isDraggedNow)
	{
		m_IsDraggedOver = false;
	}

	if (ImGui::IsWindowHovered() && m_IsDraggedOver)
	{
		ImVec2 screenPos = ImGui::GetCursorScreenPos();
		ImVec2 pos = ImGui::GetIO().MousePos;
		ImVec2 lol(pos.x - screenPos.x, pos.y - screenPos.y);
		Point p;

		lol.x = lol.x / (m_Width - 32.0f);
		lol.y = lol.y / (m_Height - ImGui::GetFontSize() * 1.7f);

		if (std::ranges::find_if(m_Paths.back().points, [&](const Point& point) { return dist(point, lol) < 0.001f; }) == m_Paths.back().points.end())
		{
			p.position = lol;
			p.color.x = Path::s_Color[0];
			p.color.y = Path::s_Color[1];
			p.color.z = Path::s_Color[2];

			m_Paths.back().points.push_back(p);
		}
	}
}

void UIScanImageWindow::RenderScanAndBrushes()
{
	m_FB->BindBuffer();
	s_ScanVAO->Bind();
	s_ScanIBO->Bind();
	m_Shader->Bind();
	m_ScanTexture->Bind();

	GLCall(glDrawElements(GL_TRIANGLES, UIScanImageWindow::s_ScanIBO->GetCount(), GL_UNSIGNED_INT, nullptr));

	size_t pointsCount = 0;

	for (const auto& path : m_Paths)
	{
		pointsCount += path.points.size();
	}

	std::vector<float> pointsData;
	std::vector<uint32_t> pointsIndices;

	std::vector<GLsizei> count;
	std::vector<GLvoid*> idcs;

	uint32_t currIndex = 0;
	uint32_t offset = 0;

	pointsData.reserve(pointsCount * 5);
	pointsIndices.reserve(pointsCount * 2);

	for (const auto& path : m_Paths)
	{
		if (!path.shoudlDraw)
		{
			continue;
		}

		int32_t indices = 0;

		idcs.push_back(BUFFER_OFFSET(offset * sizeof(uint32_t)));

		const std::vector<Point>& pts = path.points;

		for (const auto& point : pts)
		{
			pointsData.push_back(point.position.x);
			pointsData.push_back(point.position.y);
			pointsData.push_back(point.color.x);
			pointsData.push_back(point.color.y);
			pointsData.push_back(point.color.z);

			pointsIndices.push_back(currIndex++);

			++indices;
		}

		count.push_back(std::max(indices - 1, 0));
		offset += offset == 0 ? indices : std::max(indices - 1, 0);

		++currIndex;
	}

	Path::s_PathsShader->Bind();
	Path::s_PathVAO->Bind();
	Path::s_PathVBO->Bind();
	Path::s_PathIBO->Bind();

	Path::s_PathVBO->UpdateBuffer(pointsData.data(), pointsData.size() * sizeof(float));
	Path::s_PathIBO->UpdateBuffer(pointsIndices.data(), pointsIndices.size());

	GLCall(glMultiDrawElements(GL_LINE_STRIP, count.data(), GL_UNSIGNED_INT, idcs.data(), idcs.size()));

	m_FB->UnbindBuffer();
	m_FB->BindTexture(1);

	ImGui::Image((void*)(intptr_t)m_FB->GetTextureID(), ImVec2((m_Width - 32.0f), m_Height - ImGui::GetFontSize() * 1.7f - 24.0f));
}
