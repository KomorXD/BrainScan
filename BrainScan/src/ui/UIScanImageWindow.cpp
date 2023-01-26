#include "UIScanImageWindow.hpp"
#include "imgui/imgui.h"
#include "../Core.hpp"
#include "../OpenGL.hpp"

#include <format>

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
	m_ScanTexture = std::make_shared<Texture>(m_View->GetCurrentDepthData().buffer, m_View->GetWidth(), m_View->GetHeight());
}

bool UIScanImageWindow::TryToHandleScroll(double offset)
{
	if (!m_IsHovered)
	{
		return false;
	}

	bool depthChanged = offset < 0.0 ? m_View->TraverseDown() : m_View->TraverseUp();

	if (depthChanged)
	{
		m_ScanTexture = std::make_shared<Texture>(m_View->GetCurrentDepthData().buffer, m_View->GetWidth(), m_View->GetHeight());
	}

	return true;
}

void UIScanImageWindow::Render()
{
	ImGui::SetNextWindowSize(ImVec2(m_Width, m_Height));
	ImGui::SetNextWindowPos(ImVec2(m_PosX, m_PosY));

	ImGui::Begin(std::format("{} plane - Layer #{}", m_Label, m_View ? m_View->GetCurrentDepth() : 0).c_str(), nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

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

static float dist(const ImVec2& first, const ImVec2& second)
{
	return std::sqrtf(std::powf(first.x - second.x, 2) + std::powf(first.y - second.y, 2));
}

void UIScanImageWindow::CheckForDrawing()
{
	m_IsHovered = ImGui::IsWindowHovered();

	bool isDraggedNow = ImGui::IsWindowHovered() && ImGui::GetIO().MouseDown[0];
	std::vector<Path>& paths = m_View->GetCurrentDepthData().paths;

	if (!m_IsDraggedOver && isDraggedNow)
	{
		m_IsDraggedOver = true;

		paths.emplace_back();
		paths.back().pathID = paths.size() == 1 ? 1 : (++paths.rbegin())->pathID + 1;
	}
	else if (m_IsDraggedOver && !isDraggedNow)
	{
		m_IsDraggedOver = false;
	}

	if (ImGui::IsWindowHovered() && m_IsDraggedOver)
	{
		ImVec2 screenPos = ImGui::GetCursorScreenPos();
		ImVec2 pos = ImGui::GetIO().MousePos;
		ImVec2 translatedImagePos(pos.x - screenPos.x, pos.y - screenPos.y);
		Point newPoint;

		translatedImagePos.x = translatedImagePos.x / (m_Width - 32.0f);
		translatedImagePos.y = translatedImagePos.y / (m_Height - ImGui::GetFontSize() * 1.7f);

		if (std::ranges::find_if(paths.back().points, [&](const Point& point) { return dist(point.position, translatedImagePos) < 0.001f; }) == paths.back().points.end())
		{
			newPoint.position = translatedImagePos;

			newPoint.color.x = Path::s_Color[0];
			newPoint.color.y = Path::s_Color[1];
			newPoint.color.z = Path::s_Color[2];
			
			newPoint.radius = Path::s_Radius;

			float divisor = 350.0f;

			if (!paths.back().points.empty())
			{
				float distanceBetweenPoints = dist(newPoint.position, paths.back().points.back().position);

				if (distanceBetweenPoints > newPoint.radius / divisor)
				{
					ImVec2 otherPos = paths.back().points.back().position;
					ImVec2 dir = ImVec2(otherPos.x - newPoint.position.x, otherPos.y - newPoint.position.y);

					dir.x /= distanceBetweenPoints;
					dir.y /= distanceBetweenPoints;
					dir.x *= newPoint.radius / divisor;
					dir.y *= newPoint.radius / divisor;

					ImVec2 currPos(newPoint.position.x + dir.x, newPoint.position.y + dir.y);

					while (dist(otherPos, currPos) > newPoint.radius / divisor)
					{
						Point fillingPoint;

						fillingPoint.position = currPos;

						fillingPoint.color.x = Path::s_Color[0];
						fillingPoint.color.y = Path::s_Color[1];
						fillingPoint.color.z = Path::s_Color[2];

						fillingPoint.radius = Path::s_Radius;

						paths.back().points.push_back(fillingPoint);

						currPos.x += dir.x;
						currPos.y += dir.y;
					}
				}
			}

			paths.back().points.push_back(newPoint);
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
	const std::vector<Path>& paths = m_View->GetCurrentDepthData().paths;

	for (const auto& path : paths)
	{
		pointsCount += path.shoudlDraw ? path.points.size() : 0;
	}

	std::vector<float> pointsData;

	pointsData.reserve(pointsCount * 6);

	for (const auto& path : paths)
	{
		if (!path.shoudlDraw)
		{
			continue;
		}

		for (const auto& point : path.points)
		{
			pointsData.push_back(point.position.x);
			pointsData.push_back(point.position.y);

			pointsData.push_back(point.color.x);
			pointsData.push_back(point.color.y);
			pointsData.push_back(point.color.z);

			pointsData.push_back(point.radius);
		}
	}

	Path::s_PathsShader->Bind();
	Path::s_PathVAO->Bind();

	Path::s_PathVBO->UpdateBuffer(pointsData.data(), pointsData.size() * sizeof(float));

	GLCall(glDrawArrays(GL_POINTS, 0, pointsData.size() / 6));

	m_FB->UnbindBuffer();
	m_FB->BindTexture(1);

	ImGui::Image((void*)(intptr_t)m_FB->GetTextureID(), ImVec2((m_Width - 32.0f), m_Height - ImGui::GetFontSize() * 1.7f - 24.0f));
}
