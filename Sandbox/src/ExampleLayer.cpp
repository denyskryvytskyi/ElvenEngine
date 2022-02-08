#include "ExampleLayer.h"

ExampleLayer::ExampleLayer()
    : Layer("ExampleLayer"), m_CameraController(1280.0f / 720.0f)
{
    // grid
    m_VAOGrid = Elven::VertexArray::Create();

    float vertices_grid[] = {
            -0.5f, -0.5f, 0.0f, 0.36f, 0.28f, 0.55f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.36f, 0.28f, 0.55f, 1.0f,
            0.5f, 0.5f, 0.0f, 0.36f, 0.28f, 0.55f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.36f, 0.28f, 0.55f, 1.0f
    };

    Elven::VertexBuffer* vbo = Elven::VertexBuffer::Create(vertices_grid, sizeof(vertices_grid));
    Elven::VertexBufferLayout layout = {
        { Elven::BufferAttributeType::Float3, "a_Position" },
        { Elven::BufferAttributeType::Float4, "a_Color" }
    };
    vbo->SetLayout(layout);

    m_VAOGrid->AddVertexBuffer(vbo);

    uint32_t indices[] = {
        0, 1, 2, 0, 2, 3
    };
    Elven::IndexBuffer* ibo = Elven::IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

    m_VAOGrid->SetIndexBuffer(ibo);

    // triangle
    m_VAOTriangle = Elven::VertexArray::Create();

    float vertices_triangle[] = {
            -0.5f, -0.5f, 0.0f, 0.1f, 0.2f, 0.6f, 1.0f,
            0.0f, 0.5f, 0.0f, 0.6f, 0.2f, 0.2f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.1f, 0.7f, 0.1f, 1.0f,
    };

    Elven::VertexBuffer* vbo_triangle = Elven::VertexBuffer::Create(vertices_triangle, sizeof(vertices_triangle));
    Elven::VertexBufferLayout layout_triangle = {
        {Elven::BufferAttributeType::Float3, "a_Position" },
        {Elven::BufferAttributeType::Float4, "a_Color" }
    };
    vbo_triangle->SetLayout(layout_triangle);

    m_VAOTriangle->AddVertexBuffer(vbo_triangle);

    uint32_t indices_triangle[] = {
       0, 1, 2
    };
    Elven::IndexBuffer* ibo_triangle = Elven::IndexBuffer::Create(indices_triangle, sizeof(indices_triangle) / sizeof(uint32_t));

    m_VAOTriangle->SetIndexBuffer(ibo_triangle);

    // Shader
    m_Shader = m_ShaderManager.Load("example_shader", "res/shaders/shader.vert", "res/shaders/shader.frag");
}

ExampleLayer::~ExampleLayer()
{
    delete m_Shader;
    delete m_VAOGrid;
    delete m_VAOTriangle;
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

    // grid
    gdm::mat4 scale = gdm::scale(gdm::mat4(), gdm::vec3(0.1f));
    for (size_t y = 0; y < 20; y++)
    {
        for (size_t x = 0; x < 20; x++)
        {
            gdm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
            gdm::mat4 model = scale * gdm::translate(gdm::mat4(), pos);

            Elven::Renderer::Submit(m_Shader, m_VAOGrid, model);
        }
    }

    // triangle
    Elven::Renderer::Submit(m_Shader, m_VAOTriangle);
    Elven::Renderer::EndScene();
}

void ExampleLayer::OnImGuiRender()
{
}

void ExampleLayer::OnEvent(Elven::Event& event)
{
    m_CameraController.OnEvent(event);
}
