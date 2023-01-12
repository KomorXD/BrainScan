#include "EmptyScene.hpp"
#include "imgui/imgui.h"
#include "../Core.hpp"

#include "../ui/UIMenuBar.hpp"
#include "../ui/UIToolBar.hpp"
#include "../ui/UIToolSettings.hpp"
#include "../ui/UIScanImageWindow.hpp"

const std::string bartekLocation = "D:\\Projekty\\BrainScan\\NiftiFiles\\test.nii";
const std::string pawelLocation = "E:\\Projects\\ASFDA\\BrainScan\\test_nifti.nii";

EmptyScene::EmptyScene()
{
	FUNC_PROFILE();

	m_Scan.LoadFromFile(bartekLocation);

	m_FB.UnbindBuffer();

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

	m_VAO = std::make_unique<VertexArray>();
	VertexBuffer vbo(vertices.data(), 16 * sizeof(float));
	m_IBO = std::make_unique<IndexBuffer>(indices.data(), 6);

	VertexBufferLayout layout;

	layout.Push<float>(2);
	layout.Push<float>(2);

	m_VAO->AddBuffer(vbo, layout);
	m_VAO->Unbind();
	vbo.Unbind();
	m_IBO->Unbind();

	View* v = m_Scan.GetSagittal();
	
	m_Shader  = std::make_unique<Shader>("res/shaders/TextureShader.vert", "res/shaders/TextureShader.frag");
	m_AxialTexture = std::make_shared<Texture>(v->GetData()[40], v->GetWidth(), v->GetHeight());

	m_Panels.emplace_back(std::make_unique<UIMenuBar>());
	m_Panels.emplace_back(std::make_unique<UIToolBar>(m_Panels.back()->GetPosX(), m_Panels.back()->GetPosY() + m_Panels.back()->GetHeight()));

	for(int i = 0; i < 10; ++i)
	{
		auto tb = (UIToolBar*)m_Panels.back().get();

		tb->AddButton([i]()
		{
			LOG_INFO("Button #{} pressed.", i + 1);
		});
	}

	m_Panels.emplace_back(std::make_unique<UIToolSettings>(m_Panels.back()->GetPosX(), m_Panels.back()->GetPosY() + m_Panels.back()->GetHeight()));

	float scanPanelHeight = m_Panels.back()->GetHeight() / 2.0f;
	UIScanImageWindow* scanPanel = nullptr;

	m_Panels.emplace_back(std::make_unique<UIScanImageWindow>(1, m_Panels.back()->GetPosX() + m_Panels.back()->GetWidth(), m_Panels.back()->GetPosY(),								  scanPanelHeight));
	scanPanel = (UIScanImageWindow*)m_Panels.back().get();
	scanPanel->SetScanTexture(m_AxialTexture);

	m_Panels.emplace_back(std::make_unique<UIScanImageWindow>(2, m_Panels.back()->GetPosX() + m_Panels.back()->GetWidth(), m_Panels.back()->GetPosY(),								  scanPanelHeight));
	scanPanel = (UIScanImageWindow*)m_Panels.back().get();
	scanPanel->SetScanTexture(m_AxialTexture);

	m_Panels.emplace_back(std::make_unique<UIScanImageWindow>(3, m_Panels.back()->GetPosX() - m_Panels.back()->GetWidth(), m_Panels.back()->GetPosY() + m_Panels.back()->GetHeight(), scanPanelHeight));
	scanPanel = (UIScanImageWindow*)m_Panels.back().get();
	scanPanel->SetScanTexture(m_AxialTexture);

	m_Panels.emplace_back(std::make_unique<UIScanImageWindow>(4, m_Panels.back()->GetPosX() + m_Panels.back()->GetWidth(), m_Panels.back()->GetPosY(),								  scanPanelHeight));
	scanPanel = (UIScanImageWindow*)m_Panels.back().get();
	scanPanel->SetScanTexture(m_AxialTexture);

	m_FB.BindBuffer();
	m_FB.AttachTexture((uint32_t)scanPanel->GetWidth() * 3, (uint32_t)scanPanel->GetHeight() * 2);
	m_FB.UnbindBuffer();
}

void EmptyScene::Input()
{
	// TODO: handle user input
}

void EmptyScene::Update()
{
	// TODO: idk actually
}

void EmptyScene::Render()
{
	m_FB.BindBuffer();
	m_VAO->Bind();
	m_IBO->Bind();
	m_Shader->Bind();
	m_AxialTexture->Bind();

	GLCall(glDrawElements(GL_TRIANGLES, m_IBO->GetCount(), GL_UNSIGNED_INT, nullptr));

	m_FB.UnbindBuffer();
	m_FB.BindTexture(1);

	for(const auto& panel : m_Panels)
	{
		panel->Render();
	}
}

void EmptyScene::SetTool()
{
	// TODO: implement xd
}
