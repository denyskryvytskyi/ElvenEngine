#include "LightingSandbox.h"

#include <Events/TextureEvent.h>

#if EDITOR_MODE
#    include <ImGui/EditorHelpers.h>
#    include <imgui.h>
#endif

LightingSandbox::LightingSandbox()
    : m_cameraController(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f)
    , m_vao(elv::VertexArray::Create())
    , m_lightVao(elv::VertexArray::Create())
    , m_textureLoadedCallback([this](const elv::events::TextureLoadedEvent& e) { OnTextureLoaded(e); })
{
    // cube vertices
    // clang-format off
    float vertices[] = {
        // front          // normals         // uv
        -0.5, -0.5, 0.5,  0.0f, 0.0f, 1.0f,  0.0, 0.0,
        0.5, -0.5, 0.5,   0.0f, 0.0f, 1.0f,  1.0, 0.0,
        0.5, 0.5, 0.5,    0.0f, 0.0f, 1.0f,  1.0, 1.0,
        -0.5, 0.5, 0.5,   0.0f, 0.0f, 1.0f,  0.0, 1.0,
        // top            // normals         // uv
        -0.5, 0.5, 0.5,   0.0f, 1.0f, 0.0f,  0.0, 0.0,
        0.5, 0.5, 0.5,    0.0f, 1.0f, 0.0f,  1.0, 0.0,
        0.5, 0.5, -0.5,   0.0f, 1.0f, 0.0f,  1.0, 1.0,
        -0.5, 0.5, -0.5,  0.0f, 1.0f, 0.0f,  0.0, 1.0,
        // back           // normals         // uv
        0.5, -0.5, -0.5,  0.0f, 0.0f, -1.0f, 0.0, 0.0,
        -0.5, -0.5, -0.5, 0.0f, 0.0f, -1.0f, 1.0, 0.0,
        -0.5, 0.5, -0.5,  0.0f, 0.0f, -1.0f, 1.0, 1.0,
        0.5, 0.5, -0.5,   0.0f, 0.0f, -1.0f, 0.0, 1.0,
        // bottom         // normals         // uv
        -0.5, -0.5, -0.5, 0.0f, -1.0f, 0.0f, 0.0, 0.0,
        0.5, -0.5, -0.5,  0.0f, -1.0f, 0.0f, 1.0, 0.0,
        0.5, -0.5, 0.5,   0.0f, -1.0f, 0.0f, 1.0, 1.0,
        -0.5, -0.5, 0.5,  0.0f, -1.0f, 0.0f, 0.0, 1.0,
        // left           // normals         // uv
        -0.5, -0.5, -0.5, -1.0f, 0.0f, 0.0f, 0.0, 0.0,
        -0.5, -0.5, 0.5,  -1.0f, 0.0f, 0.0f, 1.0, 0.0,
        -0.5, 0.5, 0.5,   -1.0f, 0.0f, 0.0f, 1.0, 1.0,
        -0.5, 0.5, -0.5,  -1.0f, 0.0f, 0.0f, 0.0, 1.0,
        // right          // normals         // uv
        0.5, -0.5, 0.5,   1.0f, 0.0f, 0.0f,  0.0, 0.0,
        0.5, -0.5, -0.5,  1.0f, 0.0f, 0.0f,  1.0, 0.0,
        0.5, 0.5, -0.5,   1.0f, 0.0f, 0.0f,  1.0, 1.0,
        0.5, 0.5, 0.5,    1.0f, 0.0f, 0.0f,  0.0, 1.0
    };
    // clang-format on

    elv::SharedPtr<elv::VertexBuffer> vbo = elv::VertexBuffer::Create(vertices, sizeof(vertices));
    vbo->SetLayout({ { elv::BufferAttributeType::Float3 },    // pos
                     { elv::BufferAttributeType::Float3 },    // normal
                     { elv::BufferAttributeType::Float2 } }); // uv

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

    // m_shader = elv::ShaderManager::Load("colors_cube_shader", "colors.vert", "colors.frag"); // default colored cube
    m_shader = elv::ShaderManager::Load("textured_cube_shader", "textured_cube.vert", "textured_cube.frag"); // textured cube

    const uint64_t hash = elv::string_id("wooden_container");
    elv::events::Subscribe<elv::events::TextureLoadedEvent>(m_textureLoadedCallback, hash);
    elv::textures::Load("wooden_container", "wooden_container.png");

    const uint64_t hashSpecular = elv::string_id("wooden_container_specular");
    elv::events::Subscribe<elv::events::TextureLoadedEvent>(m_textureLoadedCallback, hashSpecular);
    elv::textures::Load("wooden_container_specular", "wooden_container_specular.png");

    const uint64_t hashMatrix = elv::string_id("matrix");
    elv::events::Subscribe<elv::events::TextureLoadedEvent>(m_textureLoadedCallback, hashMatrix);
    elv::textures::Load("matrix", "matrix.jpg");

    // Lights
    float lightCubeVertices[] = {
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
    elv::SharedPtr<elv::VertexBuffer> lVbo = elv::VertexBuffer::Create(lightCubeVertices, sizeof(lightCubeVertices));
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
    if (!m_texturesIsReady)
        return;

    auto& camera = m_cameraController.GetCamera();
    elv::Renderer::BeginScene(camera);

    // render cubes
    m_shader->Bind();

    elv::RenderCommand::DrawIndexed(m_vao);

    m_shader->SetVector3f("u_ViewPos", camera.GetPosition());

    // cube material
    // default colored cube shader
    /* m_shader->SetVector3f("u_Material.ambient", m_cubeMaterial.ambient);
     m_shader->SetVector3f("u_Material.diffuse", m_cubeMaterial.diffuse);
    m_shader->SetVector3f("u_Material.specular", m_cubeMaterial.specular);*/

    m_shader->SetInteger("u_Material.diffuse", 1);  // diffuse map binding
    m_shader->SetInteger("u_Material.specular", 2); // specular map binding
    m_shader->SetInteger("u_Material.emission", 3); // emission map binding
    m_shader->SetInteger("u_Material.enableEmission", 0);
    m_shader->SetFloat("u_Material.shininess", m_cubeMaterial.shininess);

    if (m_lightDemoRotationEnabled) {
        m_light.position.x = sin(m_timer.Elapsed()) * 2.0f;
        m_light.position.y = sin(m_timer.Elapsed() / 2.0f) * 1.0f;
    }

    m_shader->SetVector3f("u_Light.position", m_light.position);
    m_shader->SetVector3f("u_Light.ambient", m_light.ambient);
    m_shader->SetVector3f("u_Light.diffuse", m_light.diffuse);
    m_shader->SetVector3f("u_Light.specular", m_light.specular);

    lia::mat4 model(1.0f);
    model = lia::scale(model, lia::vec3(m_cubeScale.x, m_cubeScale.y, m_cubeScale.z))
        * lia::rotateX({ 1.0f }, lia::radians(m_cubeRotation.x))
        * lia::rotateY({ 1.0f }, lia::radians(m_cubeRotation.y))
        * lia::rotateZ({ 1.0f }, lia::radians(m_cubeRotation.z))
        * lia::translate({ 1.0f }, m_cubePosition);

    m_shader->SetMatrix4("u_InversedNormalModel", lia::inverse(model));
    elv::Renderer::Submit(m_shader, m_vao, model);

    // render lights
    m_lightShader->Bind();
    lia::mat4 lightModel(1.0f);
    lightModel = lia::scale(lightModel, lia::vec3(0.2f)) * lia::translate({ 1.0f }, m_light.position);

    elv::Renderer::Submit(m_lightShader, m_vao, lightModel);

    elv::Renderer::EndScene();
}

#if EDITOR_MODE
void LightingSandbox::OnImguiRender()
{
    ImGui::Begin("Scene properties");
    // ImGui::SetWindowSize(ImVec2(300.0f, 300.0f));

    ImGui::Text("Cube Transform");
    ImGui::Separator();
    elv::editor::DrawVec3Control("cube_pos", "Position", m_cubePosition);
    elv::editor::DrawVec3Control("cube_rotation", "Rotation", m_cubeRotation);
    elv::editor::DrawVec3Control("cube_scale", "Scale", m_cubeScale);
    ImGui::Separator();

    ImGui::Text("Cube Material");
    elv::editor::DrawRGBColorControl("ambient", m_cubeMaterial.ambient);
    elv::editor::DrawRGBColorControl("diffuse", m_cubeMaterial.diffuse);
    elv::editor::DrawRGBColorControl("specular", m_cubeMaterial.specular);
    elv::editor::DrawSliderFloat("shininess", 1.0f, 256.0f, m_cubeMaterial.shininess);
    ImGui::Separator();

    elv::editor::DrawVec3Control("light_pos", "Light Position", m_light.position);
    ImGui::Separator();

    ImGui::Text("Light Material");
    ImGui::Separator();
    elv::editor::DrawRGBColorControl("light ambient", m_light.ambient);
    elv::editor::DrawRGBColorControl("light diffuse", m_light.diffuse);
    elv::editor::DrawRGBColorControl("light specular", m_light.specular);

    ImGui::Separator();
    ImGui::Checkbox("Light demo rotation enabled", &m_lightDemoRotationEnabled);

    ImGui::End();
}
#endif

void LightingSandbox::OnDestroy()
{
}

void LightingSandbox::OnTextureLoaded(const elv::events::TextureLoadedEvent& e)
{
    auto texture = elv::textures::Get(e.textureName);
    int textureUnit = 1;
    if (e.textureName == "wooden_container_specular") {
        textureUnit = 2;
    } else if (e.textureName == "matrix") {
        textureUnit = 3;
    }
    texture->BindToUnit(textureUnit);

    ++m_texturesLoaded;

    if (m_texturesToLoad == m_texturesLoaded) {
        m_texturesIsReady = true;
    }
}
