#include "MeshModelSandbox.h"

#include <Events/TextureEvent.h>
#include <Renderer/Mesh.h>
#include <Renderer/Primitives/Cube.h>

#if EDITOR_MODE
#    include <ImGui/EditorHelpers.h>
#    include <imgui.h>
#endif

const int kCubesAmount = 10;
const int kEnvironments = 4;

const lia::vec3 kPointLightPositions[] = {
    lia::vec3(0.0f, 0.8f, 1.2f),
    lia::vec3(2.3f, -3.3f, -4.0f),
    lia::vec3(-4.0f, 2.0f, -12.0f),
    lia::vec3(0.0f, 0.0f, -3.0f)
};

const lia::vec3 kCubePositions[] = {
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

struct SpotLightEnvironmentSetting {
    lia::vec3 ambient;
    lia::vec3 diffuse;
    lia::vec3 specular;
};

struct EnvironmentMaterials {
    lia::vec4 clearColor;

    elv::DirectionalLight dirLight;
    SpotLightEnvironmentSetting spotLight;

    lia::vec3 pointLightColors[meshModel::kPointLightsAmount];
};

const EnvironmentMaterials kEnvironmenMaterials[kEnvironments] = {
    // ============== DESERT ================
    {

        // clear color
        { 0.75f, 0.52f, 0.3f, 1.0f },
        // dir light
        {
            { -0.2f, -1.0f, -0.3f }, // direction
            { 0.3f, 0.24f, 0.14f },  // ambient
            { 0.7f, 0.42f, 0.26f },  // deffuse
            { 0.5f, 0.5f, 0.5f },    // specular
        },
        // spot light
        {
            { 0.0f, 0.0f, 0.0f }, // ambient
            { 0.8f, 0.8f, 0.0f }, // deffuse
            { 0.8f, 0.8f, 0.0f }, // specular
        },
        // point lights
        {
            { 1.0f, 0.6f, 0.0f },
            { 1.0f, 0.0f, 0.0f },
            { 1.0f, 1.0f, 0.0f },
            { 0.2f, 0.2f, 1.0f } } },
    // ============== FACTORY ================
    {

        // clear color
        { 0.1f, 0.1f, 0.1f, 1.0f },
        // dir light
        {
            { -0.2f, -1.0f, -0.3f }, // direction
            { 0.05f, 0.05f, 0.1f },  // ambient
            { 0.2f, 0.2f, 0.7f },    // deffuse
            { 0.7f, 0.7f, 0.7f },    // specular
        },
        // spot light
        {
            { 0.0f, 0.0f, 0.0f }, // ambient
            { 1.0f, 1.0f, 1.0f }, // deffuse
            { 1.0f, 1.0f, 1.0f }, // specular
        },
        // point lights
        {
            { 0.2f, 0.2f, 0.6f },
            { 0.3f, 0.3f, 0.7f },
            { 0.0f, 0.0f, 0.3f },
            { 0.4f, 0.4f, 0.4f } } },
    // ============== HORROR ================
    {

        // clear color
        { 0.0f, 0.0f, 0.0f, 1.0f },
        // dir light
        {
            { -0.2f, -1.0f, -0.3f }, // direction
            { 0.0f, 0.0f, 0.0f },    // ambient
            { 0.05f, 0.05f, 0.05f }, // deffuse
            { 0.2f, 0.2f, 0.2f },    // specular
        },
        // spot light
        {
            { 0.0f, 0.0f, 0.0f }, // ambient
            { 1.0f, 1.0f, 1.0f }, // deffuse
            { 1.0f, 1.0f, 1.0f }, // specular
        },
        // point lights
        {

            { 0.1f, 0.1f, 0.1f },
            { 0.1f, 0.1f, 0.1f },
            { 0.1f, 0.1f, 0.1f },
            { 0.3f, 0.1f, 0.1f } } },
    // ============== BIOCHEMICAL LAB ================
    {

        // clear color
        { 0.9f, 0.9f, 0.9f, 1.0f },
        // dir light
        {
            { -0.2f, -1.0f, -0.3f }, // direction
            { 0.5f, 0.5f, 0.5f },    // ambient
            { 1.0f, 1.0f, 1.0f },    // deffuse
            { 1.0f, 1.0f, 1.0f },    // specular
        },
        // spot light
        {
            { 0.0f, 0.0f, 0.0f }, // ambient
            { 0.0f, 1.0f, 0.0f }, // deffuse
            { 0.0f, 1.0f, 0.0f }, // specular
        },
        // point lights
        {
            { 0.4f, 0.7f, 0.1f },
            { 0.4f, 0.7f, 0.1f },
            { 0.4f, 0.7f, 0.1f },
            { 0.4f, 0.7f, 0.1f } } }
};

MeshModelSandbox::MeshModelSandbox()
    : m_cameraController(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f)
    , m_lightCubeMesh(elv::MakeUniquePtr<elv::Cube>())
    , m_textureLoadedCallback([this](const elv::events::TextureLoadedEvent& e) { OnTextureLoaded(e); })
{
    // load textures
    const uint64_t hash = elv::string_id("wooden_container");
    elv::events::Subscribe<elv::events::TextureLoadedEvent>(m_textureLoadedCallback, hash);
    elv::textures::Load("wooden_container", "wooden_container.png");

    const uint64_t hashSpecular = elv::string_id("wooden_container_specular");
    elv::events::Subscribe<elv::events::TextureLoadedEvent>(m_textureLoadedCallback, hashSpecular);
    elv::textures::Load("wooden_container_specular", "wooden_container_specular.png");

    const uint64_t hashMatrix = elv::string_id("matrix");
    elv::events::Subscribe<elv::events::TextureLoadedEvent>(m_textureLoadedCallback, hashMatrix);
    elv::textures::Load("matrix", "matrix.jpg");

    // cube shader
    m_shader = elv::ShaderManager::Load("textured_cube", "textured_cube.vert", "mesh.frag");

    // light setup
    m_lightShader = elv::ShaderManager::Load("light_shader", "light_cube.vert", "light_cube.frag");

    for (size_t i = 0; i < meshModel::kPointLightsAmount; i++) {
        m_pointLights[i].position = kPointLightPositions[i];
    }

    // default environment
    SetEnvironment(0);
}

void MeshModelSandbox::OnUpdate(float dt)
{
    m_cameraController.OnUpdate(dt);
}

void MeshModelSandbox::OnRender(float dt)
{
    if (!m_texturesIsReady)
        return;

    elv::RenderCommand::SetClearColor(m_clearColor);
    elv::RenderCommand::Clear();

    auto& camera = m_cameraController.GetCamera();
    elv::Renderer::BeginScene(camera);

    // render cubes
    m_shader->Bind();

    m_shader->SetVector3f("u_ViewPos", camera.GetPosition());

    auto texture = elv::textures::Get("wooden_container");
    texture->BindToUnit(0);

    auto texture1 = elv::textures::Get("wooden_container_specular");
    texture->BindToUnit(1);

    auto texture2 = elv::textures::Get("matrix");
    texture->BindToUnit(2);

    // cube material
    // m_shader->SetInteger("u_Material.diffuse", 0);  // diffuse map binding
    // m_shader->SetInteger("u_Material.specular", 1); // specular map binding
    // m_shader->SetInteger("u_Material.emission", 2); // emission map binding
    m_shader->SetInteger("u_Material.enableEmission", 0);
    m_shader->SetFloat("u_Material.shininess", m_cubeShininess);

    // directional light
    m_shader->SetInteger("u_DirLightEnabled", m_DirLightEnabled);
    if (m_DirLightEnabled) {
        m_shader->SetVector3f("u_DirLight.direction", m_dirLight.direction);
        m_shader->SetVector3f("u_DirLight.ambient", m_dirLight.ambient);
        m_shader->SetVector3f("u_DirLight.diffuse", m_dirLight.diffuse);
        m_shader->SetVector3f("u_DirLight.specular", m_dirLight.specular);
    }

    // point light
    m_shader->SetInteger("u_PointLightEnabled", m_PointLightEnabled);
    if (m_PointLightEnabled) {
        for (size_t i = 0; i < meshModel::kPointLightsAmount; ++i) {
            m_shader->SetVector3f(fmt::format("u_PointLights[{}].position", i), m_pointLights[i].position);
            m_shader->SetVector3f(fmt::format("u_PointLights[{}].ambient", i), m_pointLights[i].ambient);
            m_shader->SetVector3f(fmt::format("u_PointLights[{}].diffuse", i), m_pointLights[i].diffuse);
            m_shader->SetVector3f(fmt::format("u_PointLights[{}].specular", i), m_pointLights[i].specular);
            m_shader->SetFloat(fmt::format("u_PointLights[{}].constant", i), m_pointLights[i].constant);
            m_shader->SetFloat(fmt::format("u_PointLights[{}].linear", i), m_pointLights[i].linear);
            m_shader->SetFloat(fmt::format("u_PointLights[{}].quadratic", i), m_pointLights[i].quadratic);
        }
    }

    // spotlight (flashlight in this example with camera position and view direction)
    m_shader->SetInteger("u_SpotLightEnabled", m_SpotLightEnabled);
    if (m_SpotLightEnabled) {
        m_flashlight.position = camera.GetPosition();
        m_flashlight.direction = m_cameraController.GetFront();

        m_shader->SetVector3f("u_SpotLight.position", m_flashlight.position);
        m_shader->SetVector3f("u_SpotLight.direction", m_flashlight.direction);
        m_shader->SetVector3f("u_SpotLight.ambient", m_flashlight.ambient);
        m_shader->SetVector3f("u_SpotLight.diffuse", m_flashlight.diffuse);
        m_shader->SetVector3f("u_SpotLight.specular", m_flashlight.specular);
        m_shader->SetFloat("u_SpotLight.cutOff", cos(lia::radians(m_flashlight.cutOff)));
        m_shader->SetFloat("u_SpotLight.outerCutOff", cos(lia::radians(m_flashlight.outerCutOff)));
        m_shader->SetFloat("u_SpotLight.constant", m_flashlight.constant);
        m_shader->SetFloat("u_SpotLight.linear", m_flashlight.linear);
        m_shader->SetFloat("u_SpotLight.quadratic", m_flashlight.quadratic);
    }

    // render cubes
    for (size_t i = 0; i < kCubesAmount; ++i) {
        lia::mat4 model(1.0f);
        model = lia::scale(model, lia::vec3(m_cubeScale.x, m_cubeScale.y, m_cubeScale.z))
            * lia::rotateX({ 1.0f }, lia::radians(m_cubeRotation.x))
            * lia::rotateY({ 1.0f }, lia::radians(m_cubeRotation.y))
            * lia::rotateZ({ 1.0f }, lia::radians(m_cubeRotation.z))
            * lia::translate({ 1.0f }, kCubePositions[i]);

        m_shader->SetMatrix4("u_InversedNormalModel", lia::inverse(model));
        elv::Renderer::Submit(m_shader, m_cubeMesh, model);
    }

    // render point lights
    if (m_PointLightEnabled) {
        m_lightShader->Bind();
        for (size_t i = 0; i < meshModel::kPointLightsAmount; ++i) {
            m_lightShader->SetVector3f("u_Color.ambient", m_pointLights[i].ambient);
            m_lightShader->SetVector3f("u_Color.diffuse", m_pointLights[i].diffuse);

            lia::mat4 lightModel(1.0f);
            lightModel = lia::scale(lightModel, lia::vec3(0.2f)) * lia::translate({ 1.0f }, m_pointLights[i].position);

            elv::Renderer::Submit(m_lightShader, m_lightCubeMesh, lightModel);
        }
    }

    elv::Renderer::EndScene();
}

#if EDITOR_MODE
void MeshModelSandbox::OnImguiRender()
{
    ImGui::Begin("Editor");

    if (ImGui::CollapsingHeader("Scene properties")) {
        elv::editor::DrawRGBAColorControl("Clear color", m_clearColor);

        // environment materials combo
        const char* items[] = { "Desert", "Factory", "Horror", "Biochemical lab" };
        static int currentEnvironmentIndex = 0;
        const char* combo_label = items[currentEnvironmentIndex];
        if (ImGui::BeginCombo("Environment ", combo_label)) {
            for (int i = 0; i < IM_ARRAYSIZE(items); ++i) {
                const bool is_selected = (currentEnvironmentIndex == i);
                if (ImGui::Selectable(items[i], is_selected)) {
                    currentEnvironmentIndex = i;
                    SetEnvironment(currentEnvironmentIndex);
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    }

    if (ImGui::CollapsingHeader("Cube")) {
        elv::editor::DrawSliderFloat("Cube shininess", 1.0f, 256.0f, m_cubeShininess);
    }

    if (ImGui::CollapsingHeader("Directional Light")) {
        ImGui::Checkbox("Enable Directional Light", &m_DirLightEnabled);
        ImGui::Separator();
        elv::editor::DrawVec3Control("dir_light_dir", "Direction", m_dirLight.direction);
        ImGui::Separator();
        ImGui::Text("Material:");
        elv::editor::DrawRGBColorControl("ambient##dir", m_dirLight.ambient);
        elv::editor::DrawRGBColorControl("diffuse##dir", m_dirLight.diffuse);
        elv::editor::DrawRGBColorControl("specular##dir", m_dirLight.specular);
    }

    if (ImGui::CollapsingHeader("Point Lights")) {
        ImGui::Checkbox("Enable Point Lights", &m_PointLightEnabled);
        ImGui::Separator();
        for (size_t i = 0; i < meshModel::kPointLightsAmount; ++i) {
            if (ImGui::TreeNode(fmt::format("Point light {}", i).c_str())) {
                elv::editor::DrawVec3Control(fmt::format("point_light_pos_{}", i), "Position", m_pointLights[i].position);
                ImGui::Separator();
                ImGui::Text("Point Light Material:");
                elv::editor::DrawRGBColorControl(fmt::format("ambient##point{}", i), m_pointLights[i].ambient);
                elv::editor::DrawRGBColorControl(fmt::format("diffuse##point{}", i), m_pointLights[i].diffuse);
                elv::editor::DrawRGBColorControl(fmt::format("specular##point{}", i), m_pointLights[i].specular);

                ImGui::TreePop();
            }
        }
    }

    if (ImGui::CollapsingHeader("Spot Light")) {
        ImGui::Checkbox("Enable Spot Light", &m_SpotLightEnabled);
        ImGui::Separator();
        elv::editor::DrawSliderFloat("Cut off angle", 0.0f, 180.0f, m_flashlight.cutOff);
        elv::editor::DrawSliderFloat("Outer cut off angle", 0.0f, 180.0f, m_flashlight.outerCutOff);
        ImGui::Separator();
        ImGui::Text("Material:");
        elv::editor::DrawRGBColorControl("ambient##spot", m_flashlight.ambient);
        elv::editor::DrawRGBColorControl("diffuse##spot", m_flashlight.diffuse);
        elv::editor::DrawRGBColorControl("specular##spot", m_flashlight.specular);
    }

    ImGui::End();
}
#endif

void MeshModelSandbox::OnTextureLoaded(const elv::events::TextureLoadedEvent& e)
{
    ++m_texturesLoaded;

    if (m_texturesToLoad == m_texturesLoaded) {
        m_texturesIsReady = true;
        SetupCubeMesh();
    }
}

void MeshModelSandbox::SetEnvironment(const int envIndex)
{
    const auto& env = kEnvironmenMaterials[envIndex];

    m_clearColor = env.clearColor;
    m_dirLight = env.dirLight;
    m_flashlight.ambient = env.spotLight.ambient;
    m_flashlight.diffuse = env.spotLight.diffuse;
    m_flashlight.specular = env.spotLight.specular;

    for (size_t i = 0; i < meshModel::kPointLightsAmount; ++i) {
        m_pointLights[i].ambient = env.pointLightColors[i] * 0.1f;
        m_pointLights[i].diffuse = env.pointLightColors[i];
        m_pointLights[i].specular = env.pointLightColors[i];
    }
}

void MeshModelSandbox::SetupCubeMesh()
{
    auto texture = elv::textures::Get("wooden_container");
    auto texture1 = elv::textures::Get("wooden_container_specular");

    std::vector<elv::MeshTexture> meshTextures = {
        { elv::MeshTextureType::Diffuse, texture },
        { elv::MeshTextureType::Specular, texture1 }
    };

    m_cubeMesh = elv::MakeUniquePtr<elv::Cube>();
    m_cubeMesh->SetTextures(meshTextures);
}
