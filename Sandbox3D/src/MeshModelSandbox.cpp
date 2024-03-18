#include "MeshModelSandbox.h"

#include <Core/Profiler.h>
#include <Renderer/Mesh.h>
#include <Renderer/RenderTopology.h>
#include <Resources/MeshLibrary.h>
#include <Scene/Components/StaticMeshComponent.h>

#if EDITOR_MODE
#    include <ImGui/EditorHelpers.h>
#    include <imgui.h>
#endif

const int kCubesAmount = 1;

MeshModelSandbox::MeshModelSandbox()
    : m_cameraController(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f)
    , m_lightCubeMesh(elv::gMeshLibrary.GetMesh("sphere"))
{
    // mesh shader
    m_shader = elv::ShaderManager::Load("textured_cube", "textured_cube.vert", "mesh.frag");

    // light setup
    m_lightShader = elv::ShaderManager::Load("light_shader", "light_cube.vert", "light_cube.frag");

    for (size_t i = 0; i < kPointLightsAmount; i++) {
        m_pointLights[i].position = kPointLightPositions[i];
    }

    // default environment
    SetEnvironment(0);

    // model loading
    auto& scene = elv::GetScene();

    if (false) {

        const auto robot = scene.CreateEntity();
        m_models.emplace_back(robot);
        scene.AddComponent<elv::TransformComponent>(robot, lia::vec3(20.0f, 0.0f, 0.0f), lia::vec3(1.0f));
        scene.AddComponent<elv::StaticMeshComponent>(robot, "robot", fmt::format("{}{}", elv::fileSystem::MODELS_PATH, "robot/robot.obj"));
    }

    if (true) {

        const auto walle = scene.CreateEntity();
        m_models.emplace_back(walle);
        scene.AddComponent<elv::TransformComponent>(walle, lia::vec3(3.0f, 0.55f, 0.0f), lia::vec3(1.0f));
        scene.AddComponent<elv::StaticMeshComponent>(walle, "walle", fmt::format("{}{}", elv::fileSystem::MODELS_PATH, "walle/walle.obj"));
    }

    if (false) {

        const auto test = scene.CreateEntity();
        m_models.emplace_back(test);
        scene.AddComponent<elv::TransformComponent>(test, lia::vec3(0.0f, 0.0f, 0.0f), lia::vec3(0.01f));
        auto& backpackMesh = scene.AddComponent<elv::StaticMeshComponent>(test, "test", fmt::format("{}{}", elv::fileSystem::MODELS_PATH, "backpack/backpack.fbx"));
        backpackMesh.AddMaterialTexture(elv::Material::TextureSlot::Diffuse, "albedo.jpg", fmt::format("{}{}", elv::fileSystem::MODELS_PATH, "backpack"));
        backpackMesh.AddMaterialTexture(elv::Material::TextureSlot::Specular, "metallic.jpg", fmt::format("{}{}", elv::fileSystem::MODELS_PATH, "backpack"));
    }

    const auto entity = scene.CreateEntity();
    m_models.emplace_back(entity);
    scene.AddComponent<elv::TransformComponent>(entity, lia::vec3(-2.0f, 2.0f, 0.0f), lia::vec3(0.01f));
    auto& cerberusMesh = scene.AddComponent<elv::StaticMeshComponent>(entity, "cerberus", fmt::format("{}{}", elv::fileSystem::MODELS_PATH, "cerberus/cerberus.fbx"));
    cerberusMesh.AddMaterialTexture(elv::Material::TextureSlot::Specular, "Cerberus_M.tga", fmt::format("{}{}", elv::fileSystem::MODELS_PATH, "cerberus/Textures"));

    const auto sponza = scene.CreateEntity();
    m_models.emplace_back(sponza);
    scene.AddComponent<elv::TransformComponent>(sponza, lia::vec3(0.0f), lia::vec3(0.01f));
    scene.AddComponent<elv::StaticMeshComponent>(sponza, "sponza", fmt::format("{}{}", elv::fileSystem::MODELS_PATH, "sponza/sponza.obj"));

    SetupCubes();

    const auto sphere = scene.CreateEntity();
    m_cubes.emplace_back(sphere);

    auto& transform = scene.AddComponent<elv::TransformComponent>(sphere);
    transform.pos = lia::vec3(-2.0f, 0.5f, 0.0f);
    auto& sphereMesh = scene.AddComponent<elv::StaticMeshComponent>(sphere, "sphere");
    sphereMesh.AddMaterialTexture(elv::Material::TextureSlot::Diffuse, "sphere.jpg", "assets/images");
}

void MeshModelSandbox::OnUpdate(float dt)
{
    m_cameraController.OnUpdate(dt);
}

void MeshModelSandbox::OnRender(float dt)
{
    // elv::PROFILE("APP renderer: ");
    elv::RenderCommand::SetClearColor(m_clearColor);
    elv::RenderCommand::Clear();

    auto& camera = m_cameraController.GetCamera();
    elv::Renderer::BeginScene(camera);

    m_shader->Bind();

    m_shader->SetVector3f("u_ViewPos", camera.GetPosition());

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
        for (size_t i = 0; i < kPointLightsAmount; ++i) {
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

    auto& scene = elv::GetScene();

    // render cubes
    for (auto cube : m_cubes) {
        auto& transform = scene.GetComponent<elv::TransformComponent>(cube);
        auto& cubeMesh = scene.GetComponent<elv::StaticMeshComponent>(cube);

        m_shader->SetMatrix4("u_InversedNormalModel", lia::inverse(transform.worldMatrix));
        elv::Renderer::Submit(m_shader, cubeMesh.GetMeshPtr(), transform.worldMatrix);
    }

    // render models
    for (const auto modelEntity : m_models) {
        const auto& transform = scene.GetComponent<elv::TransformComponent>(modelEntity);
        const auto& staticMesh = scene.GetComponent<elv::StaticMeshComponent>(modelEntity);

        m_shader->SetMatrix4("u_InversedNormalModel", lia::inverse(transform.worldMatrix));
        const auto& meshPtr = staticMesh.GetMeshPtr();
        if (meshPtr) {
            elv::Renderer::Submit(m_shader, staticMesh.GetMeshPtr(), transform.worldMatrix);
        }
    }

    // render point lights
    if (m_PointLightEnabled) {
        m_lightShader->Bind();
        for (size_t i = 0; i < kPointLightsAmount; ++i) {
            m_lightShader->SetVector3f("u_Color.ambient", m_pointLights[i].ambient);
            m_lightShader->SetVector3f("u_Color.diffuse", m_pointLights[i].diffuse);

            const lia::mat4 lightModel = lia::scale({ 1.0f }, lia::vec3(0.2f)) * lia::translate({ 1.0f }, m_pointLights[i].position);

            elv::Renderer::Submit(m_lightShader, m_lightCubeMesh, lightModel);
        }
    }

    elv::Renderer::EndScene();
}

void MeshModelSandbox::OnProcessInput(float dt)
{
    m_cameraController.OnProcessInput(dt);
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
        elv::editor::DrawSliderFloat("Shininess", 1.0f, 256.0f, m_cubeShininess);

        if (m_cubes.size() > 0) {
            ImGui::Text("Transform:");

            auto& scene = elv::GetScene();
            auto& cubeTransformComponent = scene.GetComponent<elv::TransformComponent>(m_cubes.at(0));
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
        }
    }

    if (ImGui::CollapsingHeader("Model")) {

        ImGui::Text("Transform:");

        auto& scene = elv::GetScene();
        auto& modelTransformComponent = scene.GetComponent<elv::TransformComponent>(m_models.at(0));
        if (elv::editor::DrawVec3Control("model_pos", "Position", modelTransformComponent.pos)) {
            modelTransformComponent.isDirty = true;
        }

        if (elv::editor::DrawVec3Control("model_rotation", "Rotation", modelTransformComponent.rotation)) {
            modelTransformComponent.isDirty = true;
        }
        if (elv::editor::DrawVec3Control("model_scale", "Scale", modelTransformComponent.scale)) {
            modelTransformComponent.isDirty = true;
        }
        ImGui::Separator();
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
        for (size_t i = 0; i < kPointLightsAmount; ++i) {
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

void MeshModelSandbox::SetEnvironment(const int envIndex)
{
    const auto& env = kEnvironmenMaterials[envIndex];

    m_clearColor = env.clearColor;
    m_dirLight = env.dirLight;
    m_flashlight.ambient = env.spotLight.ambient;
    m_flashlight.diffuse = env.spotLight.diffuse;
    m_flashlight.specular = env.spotLight.specular;

    for (size_t i = 0; i < kPointLightsAmount; ++i) {
        m_pointLights[i].ambient = env.pointLightColors[i] * 0.1f;
        m_pointLights[i].diffuse = env.pointLightColors[i];
        m_pointLights[i].specular = env.pointLightColors[i];
    }
}

void MeshModelSandbox::SetupCubes()
{
    auto& scene = elv::GetScene();

    auto mainCube = scene.CreateEntity();
    m_cubes.emplace_back(mainCube);

    auto& transform = scene.AddComponent<elv::TransformComponent>(mainCube);
    transform.pos = lia::vec3(0.0f, 0.5f, 0.0f);

    auto& mesh = scene.AddComponent<elv::StaticMeshComponent>(mainCube, "cube");
    mesh.AddMaterialTexture(elv::Material::TextureSlot::Diffuse, "wooden_container.png", "assets/images/");
    mesh.AddMaterialTexture(elv::Material::TextureSlot::Specular, "wooden_container_specular.png", "assets/images/");

    for (size_t i = 0; i < kCubesAmount; ++i) {

        auto cube = scene.CreateChildEntity(mainCube);
        m_cubes.emplace_back(cube);

        auto& transform = scene.AddComponent<elv::TransformComponent>(cube, kCubePositions[i], lia::vec3(0.5f));
        auto& childMesh = scene.AddComponent<elv::StaticMeshComponent>(cube, "cube");
        auto& childMaterial = childMesh.GetMeshPtr()->GetMaterial();
        childMesh.AddMaterialTexture(elv::Material::TextureSlot::Diffuse, "wooden_container.png", "assets/images/");
        childMesh.AddMaterialTexture(elv::Material::TextureSlot::Specular, "wooden_container_specular.png", "assets/images/");
    }
}
