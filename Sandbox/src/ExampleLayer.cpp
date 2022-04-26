#include "ExampleLayer.h"

#include <filesystem>
#include <iostream>

#include <lia/lia.h>

#include <Renderer/Shader.h>

ExampleLayer::ExampleLayer()
    : Layer("ExampleLayer"), m_CameraController(1280.0f / 720.0f)
{
}

ExampleLayer::~ExampleLayer()
{
}

void ExampleLayer::OnAttach()
{
}

void ExampleLayer::OnDetach()
{
}

void ExampleLayer::OnUpdate(float dt)
{
    m_CameraController.OnUpdate(dt);

    Elven::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
    Elven::RenderCommand::Clear();

    Elven::Renderer::Init();
    Elven::Renderer2D::Init();

    Elven::Renderer2D::BeginScene(m_CameraController.GetCamera());

    Elven::Renderer2D::DrawQuad({ -0.75f, -0.25f }, { 1.0f, 1.0f }, { 0.6f, 0.5f, 0.4f, 1.0f});

    for (size_t y = 0; y < 20; y++)
    {
        for (size_t x = 0; x < 20; x++)
        {
            lia::vec2 pos(x * 0.12f, y * 0.12f);
            lia::vec2 scale = { 0.1f, 0.1f };
            lia::vec4 color = { 0.3f, 0.7f, 0.3f, 1.0f };
            
            float angle = 0.0f;

            Elven::Renderer2D::DrawRotatedQuad(pos, scale, color, angle);
        }
    }
    Elven::Renderer2D::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
}