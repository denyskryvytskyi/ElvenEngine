#include "ExampleLayer.h"

ExampleLayer::ExampleLayer()
    : Layer("ExampleLayer"), m_CameraController(1280.0f / 720.0f)
{
    m_VAO = Elven::VertexArray::Create();

    float vertices[] = {
            -0.5f, -0.5f, 0.0f, 0.1f, 0.2f, 0.6f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.6f, 0.2f, 0.2f, 1.0f,
            0.5f, 0.5f, 0.0f, 0.1f, 0.7f, 0.1f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f
    };

    Elven::Ref<Elven::VertexBuffer> vbo = Elven::VertexBuffer::Create(vertices, sizeof(vertices));
    Elven::VertexBufferLayout layout = {
        {Elven::BufferAttributeType::Float3, "a_Position" },
        {Elven::BufferAttributeType::Float4, "a_Color" }
    };
    vbo->SetLayout(layout);

    m_VAO->AddVertexBuffer(vbo);

    uint32_t indices[] = {
        0, 1, 2, 0, 2, 3
    };
    Elven::Ref<Elven::IndexBuffer> ibo = Elven::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

    m_VAO->SetIndexBuffer(ibo);

    m_Shader = m_ShaderManager.Load("example_square_shader", "res/shaders/shader.vert", "res/shaders/shader.frag");
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

    // Render
    Elven::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
    Elven::RenderCommand::Clear();

    Elven::Renderer::BeginScene(m_CameraController.GetCamera());
    Elven::Renderer::Submit(m_Shader, m_VAO);
    Elven::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
}

void ExampleLayer::OnEvent(Elven::Event& event)
{
    m_CameraController.OnEvent(event);
}
