#include "SandboxApp.h"

#include "Events/TextureEvent.h"

#include <GLFW/glfw3.h>

Elven::Application* Elven::CreateApplication()
{
    return new Sandbox3D();
}

Sandbox3D::Sandbox3D()
    : m_cameraController(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f)
    , m_vao(Elven::VertexArray::Create())
    , m_textureLoadedCallback([this](const Elven::events::TextureLoadedEvent& e) { OnTextureLoaded(e); })
{
    // quad vertices
    float vertices[] = {
        // front         // uv
        -0.5, -0.5, 0.5, 0.0, 0.0,
        0.5, -0.5, 0.5, 1.0, 0.0,
        0.5, 0.5, 0.5, 1.0, 1.0,
        -0.5, 0.5, 0.5, 0.0, 1.0,
        // top          // uv
        -0.5, 0.5, 0.5, 0.0, 0.0,
        0.5, 0.5, 0.5, 1.0, 0.0,
        0.5, 0.5, -0.5, 1.0, 1.0,
        -0.5, 0.5, -0.5, 0.0, 1.0,
        // back         // uv
        0.5, -0.5, -0.5, 0.0, 0.0,
        -0.5, -0.5, -0.5, 1.0, 0.0,
        -0.5, 0.5, -0.5, 1.0, 1.0,
        0.5, 0.5, -0.5, 0.0, 1.0,
        // bottom        // uv
        -0.5, -0.5, -0.5, 0.0, 0.0,
        0.5, -0.5, -0.5, 1.0, 0.0,
        0.5, -0.5, 0.5, 1.0, 1.0,
        -0.5, -0.5, 0.5, 0.0, 1.0,
        // left          // uv
        -0.5, -0.5, -0.5, 0.0, 0.0,
        -0.5, -0.5, 0.5, 1.0, 0.0,
        -0.5, 0.5, 0.5, 1.0, 1.0,
        -0.5, 0.5, -0.5, 0.0, 1.0,
        // right        // uv
        0.5, -0.5, 0.5, 0.0, 0.0,
        0.5, -0.5, -0.5, 1.0, 0.0,
        0.5, 0.5, -0.5, 1.0, 1.0,
        0.5, 0.5, 0.5, 0.0, 1.0
    };

    Elven::SharedPtr<Elven::VertexBuffer> vbo = Elven::VertexBuffer::Create(vertices, sizeof(vertices));
    vbo->SetLayout({ { Elven::BufferAttributeType::Float3 },
                     { Elven::BufferAttributeType::Float2 } });

    m_vao->AddVertexBuffer(vbo);

    // clang-format off
     unsigned int indices[] = {
         // front
         0,  1,  2,
         2,  3,  0,
         // top
         4,  5,  6,
         6,  7,  4,
         // back
          8,  9, 10,
         10, 11,  8,
         // bottom
         12, 13, 14,
         14, 15, 12,
         // left
         16, 17, 18,
         18, 19, 16,
         // right
         20, 21, 22,
         22, 23, 20,
     };
    // clang-format on

    Elven::SharedPtr<Elven::IndexBuffer> ebo = Elven::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int));

    m_vao->SetIndexBuffer(ebo);

    m_shader = Elven::ShaderManager::Load("shader", "shader.vert", "shader.frag");

    const uint64_t hash = Elven::string_id("wall");
    Elven::events::Subscribe<Elven::events::TextureLoadedEvent>(m_textureLoadedCallback, hash);
    Elven::textures::Load("wall", "wall.png");

    m_cubes = {
        lia::vec3(0.0f, 0.0f, 0.0f),
        lia::vec3(2.0f, 5.0f, -15.0f),
        lia::vec3(-1.5f, -2.2f, -2.5f),
        lia::vec3(-3.8f, -2.0f, -12.3f),
        lia::vec3(2.4f, -0.4f, -3.5f),
        lia::vec3(-1.7f, 3.0f, -7.5f),
        lia::vec3(1.3f, -2.0f, -2.5f),
        lia::vec3(1.5f, 2.0f, -2.5f),
        lia::vec3(1.5f, 0.2f, -1.5f),
        lia::vec3(-1.3f, 1.0f, -1.5f)

    };
}

void Sandbox3D::OnCreate()
{
}

void Sandbox3D::OnUpdate(float dt)
{
    m_cameraController.OnUpdate(dt);
}

void Sandbox3D::OnRender(float dt)
{
    if (m_texture == nullptr)
        return;

    Elven::Renderer::BeginScene(m_cameraController.GetCamera());

    for (size_t i = 0; i < m_cubes.size(); i++) {
        lia::mat4 model = lia::rotate(lia::mat4(1.0f), (float)glfwGetTime() * (i + 1) * lia::radians(10.0f), lia::vec3(1.0f, 1.0f, 0.0f));

        model = lia::translate(model, m_cubes[i]);
        Elven::Renderer::Submit(m_shader, m_vao, model);
    }

    Elven::Renderer::EndScene();
}

void Sandbox3D::OnDestroy()
{
}

void Sandbox3D::OnTextureLoaded(const Elven::events::TextureLoadedEvent& e)
{
    m_texture = Elven::textures::Get("wall");
    m_texture->BindToUnit(0);
}
