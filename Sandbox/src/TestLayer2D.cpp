#include "TestLayer2D.h"

#include <filesystem>
#include <iostream>

#include <lia/lia.h>

#include "Elven.h"

TestLayer2D::TestLayer2D()
    : Layer("TestLayer2D")
    , m_cameraController(1280.0f / 720.0f)
{
    m_textureWizard = Elven::Texture2D::Create("wizard.png");
    m_textureWizardFire = Elven::Texture2D::Create("wizard_fire.png");
    m_textureWizardIce = Elven::Texture2D::Create("wizard_ice.png");
}

TestLayer2D::~TestLayer2D()
{
}

void TestLayer2D::OnAttach()
{

}

void TestLayer2D::OnDetach()
{
}

void TestLayer2D::OnUpdate(float dt)
{
    m_cameraController.OnUpdate(dt);

    Elven::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
    Elven::RenderCommand::Clear();

    Elven::Renderer2D::BeginScene(m_cameraController.GetCamera());

    Elven::Renderer2D::DrawRotatedQuad({ -0.5f, 0.5f }, { 0.6f, 0.6f }, 45.0f, { 0.6f, 0.5f, 0.4f, 1.0f});

    Elven::Renderer2D::DrawQuad({ -1.0f, -0.5f }, { 1.0f, 0.9f }, m_textureWizard);
    Elven::Renderer2D::DrawRotatedQuad({ -0.1f, -0.5f }, { 1.0f, 0.9f }, m_textureWizardFire, 90.0f);
    Elven::Renderer2D::DrawQuad({ 1.0f, -0.5f }, { 1.0f, 0.9f }, m_textureWizardIce);

    for (size_t y = 0; y < 20; y++)
    {
        for (size_t x = 0; x < 20; x++)
        {
            lia::vec2 pos(x * 0.12f, y * 0.12f);
            lia::vec2 scale = { 0.1f, 0.1f };
            lia::vec4 color = { 0.3f, 0.7f, 0.3f, 1.0f };

            float angle = 0.0f;

            Elven::Renderer2D::DrawRotatedQuad(pos, scale, angle, color);
        }
    }
    Elven::Renderer2D::EndScene();
}

void TestLayer2D::OnImGuiRender()
{
}