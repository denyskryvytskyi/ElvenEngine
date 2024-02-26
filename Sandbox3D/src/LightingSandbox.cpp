#include "LightingSandbox.h"

#include "Events/TextureEvent.h"

namespace {
const lia::vec3 lightPos(1.2f, 1.0f, 2.0f);
}

LightingSandbox::LightingSandbox()
    : m_cameraController(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f)
    , m_vao(elv::VertexArray::Create())
    , m_lightVao(elv::VertexArray::Create())
    , m_textureLoadedCallback([this](const elv::events::TextureLoadedEvent& e) { OnTextureLoaded(e); })
{
    // cube vertices
    float vertices[] = {
        // front
        -0.5, -0.5, 0.5,
        0.5, -0.5, 0.5,
        0.5, 0.5, 0.5,
        -0.5, 0.5, 0.5,
        // top
        -0.5, 0.5, 0.5,
        0.5, 0.5, 0.5,
        0.5, 0.5, -0.5,
        -0.5, 0.5, -0.5,
        // back
        0.5, -0.5, -0.5,
        -0.5, -0.5, -0.5,
        -0.5, 0.5, -0.5,
        0.5, 0.5, -0.5,
        // bottom
        -0.5, -0.5, -0.5,
        0.5, -0.5, -0.5,
        0.5, -0.5, 0.5,
        -0.5, -0.5, 0.5,
        // left
        -0.5, -0.5, -0.5,
        -0.5, -0.5, 0.5,
        -0.5, 0.5, 0.5,
        -0.5, 0.5, -0.5,
        // right
        0.5, -0.5, 0.5,
        0.5, -0.5, -0.5,
        0.5, 0.5, -0.5,
        0.5, 0.5, 0.5
    };

    elv::SharedPtr<elv::VertexBuffer> vbo = elv::VertexBuffer::Create(vertices, sizeof(vertices));
    vbo->SetLayout({ { elv::BufferAttributeType::Float3 } });

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

    elv::SharedPtr<elv::IndexBuffer> ebo = elv::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int));

    m_vao->SetIndexBuffer(ebo);

    m_shader = elv::ShaderManager::Load("colors_cube_shader", "colors.vert", "colors.frag");

    /*const uint64_t hash = elv::string_id("wall");
    elv::events::Subscribe<elv::events::TextureLoadedEvent>(m_textureLoadedCallback, hash);
    elv::textures::Load("wall", "wall.png");*/

    m_cubes = {
        lia::vec3(0.0f, 0.0f, 0.0f)
    };

    // Lights
    elv::SharedPtr<elv::VertexBuffer> lVbo = elv::VertexBuffer::Create(vertices, sizeof(vertices));
    lVbo->SetLayout({ { elv::BufferAttributeType::Float3 } });
    m_lightVao->AddVertexBuffer(lVbo);

    elv::SharedPtr<elv::IndexBuffer> lEbo = elv::IndexBuffer::Create(indices, sizeof(indices) / sizeof(unsigned int));
    m_lightVao->SetIndexBuffer(lEbo);

    m_lightShader = elv::ShaderManager::Load("light_shader", "light_cube.vert", "light_cube.frag");
}

void LightingSandbox::OnCreate()
{
}

void LightingSandbox::OnUpdate(float dt)
{
    m_cameraController.OnUpdate(dt);
}

void LightingSandbox::OnRender(float dt)
{
    /*if (m_texture == nullptr)
        return;*/

    elv::Renderer::BeginScene(m_cameraController.GetCamera());

    // render cubes
    m_shader->Bind();

    elv::RenderCommand::DrawIndexed(m_vao);
    for (size_t i = 0; i < m_cubes.size(); i++) {

        m_shader->SetVector3f("u_ObjectColor", 1.0f, 0.5f, 0.31f);
        m_shader->SetVector3f("u_LightColor", 1.0f, 1.0f, 1.0f);

        lia::mat4 model(1.0f);
        // model = lia::translate(model, m_cubes[i]);
        elv::Renderer::Submit(m_shader, m_vao, model);
    }

    // render lights
    m_lightShader->Bind();
    lia::mat4 model(1.0f);
    model = lia::scale(model, lia::vec3(0.2f)); // a smaller cube
    model = lia::translate(model, lightPos);
    elv::Renderer::Submit(m_shader, m_vao, model);

    elv::Renderer::EndScene();
}

void LightingSandbox::OnDestroy()
{
}

void LightingSandbox::OnTextureLoaded(const elv::events::TextureLoadedEvent& e)
{
    /* m_texture = elv::textures::Get("wall");
     m_texture->BindToUnit(1);*/
}
