#include "UIScanImageWindow.hpp"
#include "imgui/imgui.h"
#include "../Core.hpp"
#include "../OpenGL.hpp"

#include <format>

#define BUFFER_OFFSET(offset) (static_cast<uint8_t*>(0) + (offset))

std::unique_ptr<VertexArray> UIScanImageWindow::s_ScanVAO;
std::unique_ptr<IndexBuffer> UIScanImageWindow::s_ScanIBO;

UIScanImageWindow::UIScanImageWindow(const std::string& label, float posX, float posY, float height)
	: IUIPanel(posX, posY), m_Label(label)
{
	m_FB = std::make_shared<Framebuffer>();
	m_FB->UnbindBuffer();

	ImVec2 windowSize = ImGui::GetMainViewport()->Size;

	m_Width = windowSize.x / 3.0f;
	m_Height = height;

	LOG_INFO("Initialized {} scan panel", label);

	m_FB->BindBuffer();
	m_FB->AttachTexture((uint32_t)m_Width * 3, (uint32_t)m_Height * 2);
	m_FB->UnbindBuffer();
}

//void UIScanImageWindow::SetScanTexture(std::shared_ptr<Texture>& text)
//{
//	m_ScanTexture = text;
//}

void UIScanImageWindow::SetShader(std::shared_ptr<Shader>& shader)
{
	m_Shader = shader;
}

void UIScanImageWindow::SetView(View* view)
{
	if (!view)
	{
		return;
	}

	m_View = view;
	m_Depth = view->GetDepth() / 2;
	m_ScanTexture = std::make_shared<Texture>(view->GetData()[m_Depth].buffer, view->GetWidth(), view->GetHeight());
}

void UIScanImageWindow::Render()
{
	ImGui::SetNextWindowSize(ImVec2(m_Width, m_Height));
	ImGui::SetNextWindowPos(ImVec2(m_PosX, m_PosY));

	ImGui::Begin(std::format("Scan - {} plane", m_Label).c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

	Update();

	ImGui::BeginChild(m_Label.c_str(), ImVec2(m_Width - 16.0f, m_Height - ImGui::GetFontSize() * 1.7f - 8.0f), true);

	if (s_DrawingEnabled && m_View)
	{
		CheckForDrawing();
	}

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

static float dist(const Point& point1, const ImVec2& vect2)
{
	return std::sqrtf(std::powf(point1.position.x - vect2.x, 2) + std::powf(point1.position.y - vect2.y, 2));
}

void UIScanImageWindow::CheckForDrawing()
{
	bool isDraggedNow = ImGui::IsWindowHovered() && ImGui::GetIO().MouseDown[0];
	std::vector<Path>& paths = m_View->GetData().at(m_Depth).paths;

	if (!m_IsDraggedOver && isDraggedNow)
	{
		m_IsDraggedOver = true;

		paths.emplace_back();
		paths.back().pathID = paths.size() == 1 ? 1 : (++paths.rbegin())->pathID + 1;
	}
	else if (m_IsDraggedOver && !isDraggedNow)
	{
		m_IsDraggedOver = false;

		if (paths.back().points.size() < 2)
		{
			paths.back().points.pop_back();
		}
	}

	if (ImGui::IsWindowHovered() && m_IsDraggedOver)
	{
		ImVec2 screenPos = ImGui::GetCursorScreenPos();
		ImVec2 pos = ImGui::GetIO().MousePos;
		ImVec2 lol(pos.x - screenPos.x, pos.y - screenPos.y);
		Point p;

		lol.x = lol.x / (m_Width - 32.0f);
		lol.y = lol.y / (m_Height - ImGui::GetFontSize() * 1.7f);

		if (std::ranges::find_if(paths.back().points, [&](const Point& point) { return dist(point, lol) < 0.001f; }) == paths.back().points.end())
		{
			p.position = lol;
			p.color.x = Path::s_Color[0];
			p.color.y = Path::s_Color[1];
			p.color.z = Path::s_Color[2];

			paths.back().points.push_back(p);
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
	std::vector<Path>& paths = m_View->GetData().at(m_Depth).paths;

	for (const auto& path : paths)
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

	for (const auto& path : paths)
	{
		if (!path.shoudlDraw || path.points.size() < 2)
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
