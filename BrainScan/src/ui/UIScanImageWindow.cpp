#include "UIScanImageWindow.hpp"
#include "imgui/imgui.h"
#include "../Core.hpp"
#include "../OpenGL.hpp"

#include <format>

std::unique_ptr<VertexArray> UIScanImageWindow::VAO;
std::unique_ptr<IndexBuffer> UIScanImageWindow::IBO;

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
	m_ImageRatio = ratio;
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

	ImGui::BeginChild(std::format("scan#{}", m_ID).c_str(), ImVec2((m_Width - 16.0f) * m_ImageRatio, m_Height - ImGui::GetFontSize() * 1.7f - 8.0f), true);

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

		lol.x = lol.x / ((m_Width - 32.0f) * m_ImageRatio);
		lol.y = lol.y / (m_Height - ImGui::GetFontSize() * 1.7f);

		if (std::ranges::find_if(m_Paths.back().points, [&](const Point& point) { return dist(point, lol) < 0.001f; }) == m_Paths.back().points.end())
		{
			p.position = lol;

			m_Paths.back().points.push_back(p);
		}
	}

	if (m_ScanTexture)
	{
		m_FB->BindBuffer();
		VAO->Bind();
		IBO->Bind();
		m_Shader->Bind();
		m_ScanTexture->Bind();

		int32_t totalPoints = 0;
		size_t itr = 0;

		for (const auto& path : m_Paths)
		{
			if (!path.shoudlDraw)
			{
				continue;
			}

			if (itr >= 256)
			{
				break;
			}

			const std::vector<Point>& pts = path.points;

			for (size_t i = 0; i < pts.size() && itr < 256; ++i, ++itr)
			{
				m_Shader->SetUniform1f("u_BrushPoints[" + std::to_string(itr) + "].x", pts[i].position.x);
				m_Shader->SetUniform1f("u_BrushPoints[" + std::to_string(itr) + "].y", pts[i].position.y);

				++totalPoints;
			}
		}

		m_Shader->SetUniform1i("u_BrushPointsSize", totalPoints);

		GLCall(glDrawElements(GL_TRIANGLES, IBO->GetCount(), GL_UNSIGNED_INT, nullptr));

		m_FB->UnbindBuffer();
		m_FB->BindTexture(1);

		ImGui::Image((void*)(intptr_t)m_FB->GetTextureID(), ImVec2((m_Width - 32.0f) * m_ImageRatio, m_Height - ImGui::GetFontSize() * 1.7f - 24.0f));
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

	VAO = std::make_unique<VertexArray>();
	VertexBuffer vbo(vertices.data(), 16 * sizeof(float));
	IBO = std::make_unique<IndexBuffer>(indices.data(), 6);

	VertexBufferLayout layout;

	layout.Push<float>(2);
	layout.Push<float>(2);

	VAO->AddBuffer(vbo, layout);
	VAO->Unbind();
	vbo.Unbind();
	IBO->Unbind();
}
