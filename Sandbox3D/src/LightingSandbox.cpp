#include "LightingSandbox.h"

#include "Events/TextureEvent.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace {

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

    /*const uint64_t hash = elv::string_id("wall");
    elv::events::Subscribe<elv::events::TextureLoadedEvent>(m_textureLoadedCallback, hash);
    elv::textures::Load("wall", "wall.png");*/

    m_cubes = {
        lia::vec3(0.0f, 0.0f, 0.0f)
    };

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
    /*if (m_texture == nullptr)
        return;*/

    auto& camera = m_cameraController.GetCamera();
    elv::Renderer::BeginScene(camera);

    // render cubes
    m_shader->Bind();

    elv::RenderCommand::DrawIndexed(m_vao);
    for (size_t i = 0; i < m_cubes.size(); i++) {

        m_shader->SetVector3f("u_ObjectColor", 1.0f, 0.5f, 0.31f);
        m_shader->SetVector3f("u_LightColor", 1.0f, 1.0f, 1.0f);
        m_shader->SetVector3f("u_LightPos", m_lightPos);
        m_shader->SetVector3f("u_ViewPos", camera.GetPosition());

        lia::mat4 model(1.0f);
        // m_shader->SetMatrix4("u_InversedNormalModel", lia::inverse(model));
        //  model = lia::translate(model, m_cubes[i]);
        elv::Renderer::Submit(m_shader, m_vao, model);
    }

    // render lights
    m_lightShader->Bind();
    lia::mat4 model(1.0f);
    model = lia::scale(model, lia::vec3(0.2f)); // a smaller cube
    model = lia::translate(model, m_lightPos);
    elv::Renderer::Submit(m_shader, m_vao, model);

    elv::Renderer::EndScene();
}

void LightingSandbox::OnImguiRender()
{
    float columnWidth = 150.f;
    std::string label = "light";
    float resetValue = 0.0f;

    //
    ImGui::Begin("Light");
    // ImGui::SetWindowSize(ImVec2(300.0f, 200.0f));

    ImGuiIO& io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID(label.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, 50);
    ImGui::Text(label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 0 });

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 { 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 { 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize))
        m_lightPos.x = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##X", &m_lightPos.x, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 { 0.3f, 0.8f, 0.3f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 { 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize))
        m_lightPos.y = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Y", &m_lightPos.y, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 { 0.2f, 0.35f, 0.9f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 { 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Z", buttonSize))
        m_lightPos.z = resetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    ImGui::DragFloat("##Z", &m_lightPos.z, 0.1f, 0.0f, 0.0f, "%.2f");
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);

    ImGui::PopID();

    ImGui::End();
}

void LightingSandbox::OnDestroy()
{
}

void LightingSandbox::OnTextureLoaded(const elv::events::TextureLoadedEvent& e)
{
    /* m_texture = elv::textures::Get("wall");
     m_texture->BindToUnit(1);*/
}
