#include "SimpleLightSandbox.h"

#if EDITOR_MODE
#    include <ImGui/EditorHelpers.h>
#    include <imgui.h>
#endif

SimpleLightSandbox::SimpleLightSandbox()
    : m_cameraController(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f)
    , m_vao(elv::VertexArray::Create())
    , m_lightVao(elv::VertexArray::Create())
{
    // cube vertices
    float vertices[] = {
        // front
        -0.5, -0.5, 0.5, 0.0f, 0.0f, 1.0f,
        0.5, -0.5, 0.5, 0.0f, 0.0f, 1.0f,
        0.5, 0.5, 0.5, 0.0f, 0.0f, 1.0f,
        -0.5, 0.5, 0.5, 0.0f, 0.0f, 1.0f,
        // top
        -0.5, 0.5, 0.5, 0.0f, 1.0f, 0.0f,
        0.5, 0.5, 0.5, 0.0f, 1.0f, 0.0f,
        0.5, 0.5, -0.5, 0.0f, 1.0f, 0.0f,
        -0.5, 0.5, -0.5, 0.0f, 1.0f, 0.0f,
        // back
        0.5, -0.5, -0.5, 0.0f, 0.0f, -1.0f,
        -0.5, -0.5, -0.5, 0.0f, 0.0f, -1.0f,
        -0.5, 0.5, -0.5, 0.0f, 0.0f, -1.0f,
        0.5, 0.5, -0.5, 0.0f, 0.0f, -1.0f,
        // bottom
        -0.5, -0.5, -0.5, 0.0f, -1.0f, 0.0f,
        0.5, -0.5, -0.5, 0.0f, -1.0f, 0.0f,
        0.5, -0.5, 0.5, 0.0f, -1.0f, 0.0f,
        -0.5, -0.5, 0.5, 0.0f, -1.0f, 0.0f,
        // left
        -0.5, -0.5, -0.5, -1.0f, 0.0f, 0.0f,
        -0.5, -0.5, 0.5, -1.0f, 0.0f, 0.0f,
        -0.5, 0.5, 0.5, -1.0f, 0.0f, 0.0f,
        -0.5, 0.5, -0.5, -1.0f, 0.0f, 0.0f,
        // right
        0.5, -0.5, 0.5, 1.0f, 0.0f, 0.0f,
        0.5, -0.5, -0.5, 1.0f, 0.0f, 0.0f,
        0.5, 0.5, -0.5, 1.0f, 0.0f, 0.0f,
        0.5, 0.5, 0.5, 1.0f, 0.0f, 0.0f
    };

    elv::SharedPtr<elv::VertexBuffer> vbo = elv::VertexBuffer::Create(vertices, sizeof(vertices));
    vbo->SetLayout({ { elv::BufferAttributeType::Float3 },    // pos
                     { elv::BufferAttributeType::Float3 } }); // normal

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

    // scene setup
    auto& scene = elv::GetScene();

    m_cubeEntity = scene.CreateEntity();
    scene.AddComponent<elv::TransformComponent>(m_cubeEntity);

    m_cubeMaterial.SetAmbientColor({ 1.0f, 0.5f, 0.31f });
    m_cubeMaterial.SetDiffuseColor({ 1.0f, 0.5f, 0.31f });
    m_cubeMaterial.SetSpecularColor({ 0.5f });
    m_cubeMaterial.SetShininess(32.0f);

    m_lightEntity = scene.CreateEntity();
    scene.AddComponent<elv::TransformComponent>(m_lightEntity, m_light.position, lia::vec3(0.2f));
}

void SimpleLightSandbox::OnUpdate(float dt)
{
    m_cameraController.OnUpdate(dt);
}

void SimpleLightSandbox::OnRender(float dt)
{
    auto& camera = m_cameraController.GetCamera();
    elv::Renderer::BeginScene(camera);

    auto& scene = elv::GetScene();
    auto& cubeTransformComponent = scene.GetComponent<elv::TransformComponent>(m_cubeEntity);
    auto& lightTransform = scene.GetComponent<elv::TransformComponent>(m_lightEntity);

    // render cubes
    m_shader->Bind();

    m_shader->SetVector3f("u_ViewPos", camera.GetPosition());

    // cube material
    m_shader->SetVector3f("u_Material.ambient", m_cubeMaterial.GetAmbientColor());
    m_shader->SetVector3f("u_Material.diffuse", m_cubeMaterial.GetDiffuseColor());
    m_shader->SetVector3f("u_Material.specular", m_cubeMaterial.GetSpecularColor());
    m_shader->SetFloat("u_Material.shininess", m_cubeMaterial.GetShininess());

    if (m_lightDemoRotationEnabled) {
        m_light.position.x = sin(m_timer.Elapsed()) * 2.0f;
        m_light.position.y = sin(m_timer.Elapsed() / 2.0f) * 1.0f;
    }

    m_shader->SetVector3f("u_Light.position", lightTransform.pos);
    m_shader->SetVector3f("u_Light.ambient", m_light.ambient);
    m_shader->SetVector3f("u_Light.diffuse", m_light.diffuse);
    m_shader->SetVector3f("u_Light.specular", m_light.specular);

    m_shader->SetMatrix4("u_InversedNormalModel", lia::inverse(cubeTransformComponent.worldMatrix));
    elv::Renderer::Submit(m_shader, m_vao, cubeTransformComponent.worldMatrix);

    // render light
    m_lightShader->Bind();
    m_lightShader->SetVector3f("u_Color.ambient", m_light.ambient);
    m_lightShader->SetVector3f("u_Color.diffuse", m_light.diffuse);

    elv::Renderer::Submit(m_lightShader, m_vao, lightTransform.worldMatrix);

    elv::Renderer::EndScene();
}

void SimpleLightSandbox::OnProcessInput(float dt)
{
    m_cameraController.OnProcessInput(dt);
}

#if EDITOR_MODE
void SimpleLightSandbox::OnImguiRender()
{
    auto& scene = elv::GetScene();

    ImGui::Begin("Scene properties");

    ImGui::Text("Cube Transform");
    ImGui::Separator();

    auto& cubeTransformComponent = scene.GetComponent<elv::TransformComponent>(m_cubeEntity);
    if (elv::editor::DrawVec3Control("cube_pos", "Position", cubeTransformComponent.pos)) {
        cubeTransformComponent.isDirty = true;
    }

    if (elv::editor::DrawVec3Control("cube_rotation", "Rotation", cubeTransformComponent.rotation)) {
        cubeTransformComponent.isDirty = true;
    }
    if (elv::editor::DrawVec3Control("cube_scale", "Scale", cubeTransformComponent.scale)) {
        cubeTransformComponent.isDirty = true;
    }
    ImGui::Separator();

    ImGui::Text("Cube Material");
    auto ambient = m_cubeMaterial.GetAmbientColor();
    auto diffuse = m_cubeMaterial.GetDiffuseColor();
    auto specular = m_cubeMaterial.GetSpecularColor();
    auto shininess = m_cubeMaterial.GetShininess();

    if (elv::editor::DrawRGBColorControl("ambient", ambient)) {
        m_cubeMaterial.SetAmbientColor(ambient);
    }
    if (elv ::editor::DrawRGBColorControl("diffuse", diffuse)) {
        m_cubeMaterial.SetDiffuseColor(diffuse);
    }
    if (elv::editor::DrawRGBColorControl("specular", specular)) {
        m_cubeMaterial.SetSpecularColor(specular);
    }
    if (elv::editor::DrawSliderFloat("shininess", 1.0f, 256.0f, shininess)) {
        m_cubeMaterial.SetShininess(shininess);
    }

    ImGui::Separator();

    auto& lightTransform = scene.GetComponent<elv::TransformComponent>(m_lightEntity);
    if (elv::editor::DrawVec3Control("light_pos", "Light Position", lightTransform.pos)) {
        lightTransform.isDirty = true;
    }
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
