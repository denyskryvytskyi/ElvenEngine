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
    m_textureBackground = Elven::Texture2D::Create("game_background_1.png");
}

TestLayer2D::~TestLayer2D()
{
}

void TestLayer2D::OnAttach()
{
    // TODO: Remove to application according to user settings
    Elven::Renderer2D::Init();
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

    Elven::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.5f }, { 3.84f, 2.16f }, m_textureBackground);
    Elven::Renderer2D::DrawQuad({ -1.0f, 0.0f, 0.1f }, { 1.0f, 0.9f }, m_textureWizard);
    Elven::Renderer2D::DrawQuad({ 0.0f, -0.1f, 0.1f }, { 1.0f, 0.9f }, m_textureWizardIce, { 1.0f, 1.0f, 1.0f, 1.0f });
    Elven::Renderer2D::DrawRotatedQuad({ 1.0f, 0.2f, 0.1f }, { 1.0f, 0.9f }, { 0.0f, 180.0f, 0.0f }, m_textureWizardFire);

    TestDrawQuads(false);

    Elven::Renderer2D::EndScene();
}

void TestLayer2D::OnImGuiRender()
{
}

void TestLayer2D::TestDrawQuads(bool draw)
{
    if (draw) {
        Elven::Renderer2D::DrawRotatedQuad({ -0.5f, 0.5f, 0.0f }, { 0.6f, 0.6f }, { 0.0f, 0.0f, 45.0f }, { 0.6f, 0.5f, 0.4f, 1.0f });

        const float angleZ = 45.0f;

        for (size_t y = 0; y < 20; y++) {
            for (size_t x = 0; x < 20; x++) {
                lia::vec3 pos(x * 0.12f, y * 0.12f, 0.0f);
                lia::vec2 scale = { 0.1f, 0.1f };
                lia::vec4 color = { 0.3f, 0.7f, 0.3f, 1.0f };

                Elven::Renderer2D::DrawRotatedQuad(pos, scale, { 0.0f, 0.0f, angleZ }, color);
            }
        }
    }
}
