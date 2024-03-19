#include "MeshModelSandbox.h"

#include <Core/Profiler.h>
#include <Renderer/Mesh.h>
#include <Renderer/RenderTopology.h>
#include <Resources/MeshLibrary.h>
#include <Scene/Components/LightComponent.h>
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

    // models
    auto& scene = elv::GetScene();

    if (true) {

        const auto walle = scene.CreateEntity();
        m_models.emplace_back(walle);
        scene.AddComponent<elv::TransformComponent>(walle, lia::vec3(8.0f, 0.55f, 0.0f), lia::vec3(1.0f));
        scene.AddComponent<elv::StaticMeshComponent>(walle, "walle", fmt::format("{}{}", elv::fileSystem::MODELS_PATH, "adam/adamHead.gltf"));
    }

    if (false) {

        const auto test = scene.CreateEntity();
        m_models.emplace_back(test);
        scene.AddComponent<elv::TransformComponent>(test, lia::vec3(3.0f, 0.0f, 0.0f), lia::vec3(0.01f));
        auto& backpackMesh = scene.AddComponent<elv::StaticMeshComponent>(test, "test", fmt::format("{}{}", elv::fileSystem::MODELS_PATH, "backpack_gltf/scene.gltf"));
    }

    if (false) {

        const auto robot = scene.CreateEntity();
        m_models.emplace_back(robot);
        scene.AddComponent<elv::TransformComponent>(robot, lia::vec3(20.0f, 0.0f, 0.0f), lia::vec3(1.0f));
        scene.AddComponent<elv::StaticMeshComponent>(robot, "robot", fmt::format("{}{}", elv::fileSystem::MODELS_PATH, "robot/robot.obj"));
    }

    const auto entity = scene.CreateEntity();
    m_models.emplace_back(entity);
    scene.AddComponent<elv::TransformComponent>(entity, lia::vec3(-2.0f, 2.0f, 0.0f), lia::vec3(0.01f));
    auto& cerberusMesh = scene.AddComponent<elv::StaticMeshComponent>(entity, "cerberus", fmt::format("{}{}", elv::fileSystem::MODELS_PATH, "cerberus/cerberus.fbx"));
    cerberusMesh.AddMaterialTexture(elv::Material::TextureSlot::Specular, "Cerberus_M.tga", fmt::format("{}{}", elv::fileSystem::MODELS_PATH, "cerberus/Textures"));

    const auto sponza = scene.CreateEntity();
    m_models.emplace_back(sponza);
    scene.AddComponent<elv::TransformComponent>(sponza, lia::vec3(0.0f), lia::vec3(0.1f));
    scene.AddComponent<elv::StaticMeshComponent>(sponza, "sponza", fmt::format("{}{}", elv::fileSystem::MODELS_PATH, "sponza/sponza.obj"));

    // cubes
    SetupCubes();

    // sphere
    const auto sphere = scene.CreateEntity();
    m_primitives.emplace_back(sphere);

    auto& transform = scene.AddComponent<elv::TransformComponent>(sphere);
    transform.pos = lia::vec3(-2.0f, 0.5f, 0.0f);
    auto& sphereMesh = scene.AddComponent<elv::StaticMeshComponent>(sphere, "sphere");
    auto& sphereMaterial = sphereMesh.GetMaterial();
    sphereMaterial.SetAmbientColor(lia::vec3(0.0f, 1.0f, 0.0f));
    sphereMaterial.SetDiffuseColor(lia::vec3(0.0f, 1.0f, 0.0f));
    sphereMaterial.SetSpecularColor(lia::vec3(0.0f, 1.0f, 0.0f));
    sphereMesh.AddMaterialTexture(elv::Material::TextureSlot::Specular, "white", "");
    //  sphereMesh.AddMaterialTexture(elv::Material::TextureSlot::Diffuse, "sphere.jpg", "assets/images");

    // =================== LIGHT =======================
    m_dirLightEntity = scene.CreateEntity();
    scene.AddComponent<elv::TransformComponent>(m_dirLightEntity);
    scene.AddComponent<elv::DirectionalLightComponent>(m_dirLightEntity);

    m_spotLightEntity = scene.CreateEntity();
    scene.AddComponent<elv::TransformComponent>(m_spotLightEntity);
    scene.AddComponent<elv::SpotLightComponent>(m_spotLightEntity);

    for (size_t i = 0; i < kPointLightsAmount; ++i) {
        m_pointLightEntities[i] = scene.CreateEntity();
        scene.AddComponent<elv::PointLightComponent>(m_pointLightEntities[i]);
        scene.AddComponent<elv::TransformComponent>(m_pointLightEntities[i], kPointLightPositions[i]);
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
    // elv::PROFILE("APP renderer: ");
    elv::RenderCommand::SetClearColor(m_clearColor);
    elv::RenderCommand::Clear();

    auto& camera = m_cameraController.GetCamera();
    elv::Renderer::BeginScene(camera);

    m_shader->Bind();
    m_shader->SetVector3f("u_ViewPos", camera.GetPosition());

    auto& scene = elv::GetScene();
    // ================== LIGHT ===========================

    // directional light
    m_shader->SetInteger("u_DirLightEnabled", m_DirLightEnabled);
    if (m_DirLightEnabled) {
        auto& dirLight = scene.GetComponent<elv::DirectionalLightComponent>(m_dirLightEntity);
        auto& dirLightTr = scene.GetComponent<elv::TransformComponent>(m_dirLightEntity);

        m_shader->SetVector3f("u_DirLight.direction", dirLightTr.rotation);
        m_shader->SetVector3f("u_DirLight.ambient", dirLight.ambient);
        m_shader->SetVector3f("u_DirLight.diffuse", dirLight.diffuse);
        m_shader->SetVector3f("u_DirLight.specular", dirLight.specular);
    }

    // point light
    m_shader->SetInteger("u_PointLightEnabled", m_PointLightEnabled);
    if (m_PointLightEnabled) {
        for (size_t i = 0; i < kPointLightsAmount; ++i) {
            auto& pointLight = scene.GetComponent<elv::PointLightComponent>(m_pointLightEntities[i]);
            auto& pointLightTr = scene.GetComponent<elv::TransformComponent>(m_pointLightEntities[i]);

            m_shader->SetVector3f(fmt::format("u_PointLights[{}].position", i), pointLightTr.pos);
            m_shader->SetVector3f(fmt::format("u_PointLights[{}].ambient", i), pointLight.ambient);
            m_shader->SetVector3f(fmt::format("u_PointLights[{}].diffuse", i), pointLight.diffuse);
            m_shader->SetVector3f(fmt::format("u_PointLights[{}].specular", i), pointLight.specular);
            m_shader->SetFloat(fmt::format("u_PointLights[{}].constant", i), pointLight.constant);
            m_shader->SetFloat(fmt::format("u_PointLights[{}].linear", i), pointLight.linear);
            m_shader->SetFloat(fmt::format("u_PointLights[{}].quadratic", i), pointLight.quadratic);
        }
    }

    // spotlight (flashlight in this example with camera position and view direction)
    m_shader->SetInteger("u_SpotLightEnabled", m_SpotLightEnabled);
    if (m_SpotLightEnabled) {
        auto& flashLight = scene.GetComponent<elv::SpotLightComponent>(m_spotLightEntity);
        auto& flashLightTr = scene.GetComponent<elv::TransformComponent>(m_spotLightEntity);

        flashLightTr.pos = camera.GetPosition();
        flashLightTr.rotation = m_cameraController.GetFront();

        m_shader->SetVector3f("u_SpotLight.position", flashLightTr.pos);
        m_shader->SetVector3f("u_SpotLight.direction", flashLightTr.rotation);
        m_shader->SetVector3f("u_SpotLight.ambient", flashLight.ambient);
        m_shader->SetVector3f("u_SpotLight.diffuse", flashLight.diffuse);
        m_shader->SetVector3f("u_SpotLight.specular", flashLight.specular);
        m_shader->SetFloat("u_SpotLight.cutOff", cos(lia::radians(flashLight.cutOff)));
        m_shader->SetFloat("u_SpotLight.outerCutOff", cos(lia::radians(flashLight.outerCutOff)));
        m_shader->SetFloat("u_SpotLight.constant", flashLight.constant);
        m_shader->SetFloat("u_SpotLight.linear", flashLight.linear);
        m_shader->SetFloat("u_SpotLight.quadratic", flashLight.quadratic);
    }

    // ===================================================

    // render primitives
    for (auto primitive : m_primitives) {
        auto& transform = scene.GetComponent<elv::TransformComponent>(primitive);
        auto& primitiveMesh = scene.GetComponent<elv::StaticMeshComponent>(primitive);

        m_shader->SetMatrix4("u_InversedNormalModel", lia::inverse(transform.worldMatrix));
        elv::Renderer::Submit(m_shader, primitiveMesh.GetMeshPtr(), transform.worldMatrix);
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
            auto& pointLight = scene.GetComponent<elv::PointLightComponent>(m_pointLightEntities[i]);
            auto& pointLightTr = scene.GetComponent<elv::TransformComponent>(m_pointLightEntities[i]);

            m_lightShader->SetVector3f("u_Color.ambient", pointLight.ambient);
            m_lightShader->SetVector3f("u_Color.diffuse", pointLight.diffuse);

            elv::Renderer::Submit(m_lightShader, m_lightCubeMesh, pointLightTr.worldMatrix);
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
    auto& scene = elv::GetScene();

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
        if (m_primitives.size() > 0) {
            ImGui::Text("Transform:");

            auto& scene = elv::GetScene();
            auto& cubeTransformComponent = scene.GetComponent<elv::TransformComponent>(m_primitives.at(0));
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

    if (ImGui::CollapsingHeader("Cube child")) {
        if (m_primitives.size() > 0) {
            ImGui::Text("Transform:");

            auto& scene = elv::GetScene();
            auto& cubeTransformComponent = scene.GetComponent<elv::TransformComponent>(m_primitives.at(1));
            if (elv::editor::DrawVec3Control("cube_pos#child", "Position", cubeTransformComponent.pos)) {
                cubeTransformComponent.isDirty = true;
            }

            if (elv::editor::DrawVec3Control("cube_rotation#child", "Rotation", cubeTransformComponent.rotation)) {
                cubeTransformComponent.isDirty = true;
            }
            if (elv::editor::DrawVec3Control("cube_scale#child", "Scale", cubeTransformComponent.scale)) {
                cubeTransformComponent.isDirty = true;
            }
            ImGui::Separator();
        }
    }

    if (ImGui::CollapsingHeader("Model")) {

        ImGui::Text("Transform:");

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

        auto& dirLight = scene.GetComponent<elv::DirectionalLightComponent>(m_dirLightEntity);
        auto& dirLightTr = scene.GetComponent<elv::TransformComponent>(m_dirLightEntity);

        elv::editor::DrawVec3Control("dir_light_dir", "Direction", dirLightTr.rotation);
        ImGui::Separator();

        ImGui::Text("Material:");
        elv::editor::DrawRGBColorControl("ambient##dir", dirLight.ambient);
        elv::editor::DrawRGBColorControl("diffuse##dir", dirLight.diffuse);
        elv::editor::DrawRGBColorControl("specular##dir", dirLight.specular);
    }

    if (ImGui::CollapsingHeader("Point Lights")) {
        ImGui::Checkbox("Enable Point Lights", &m_PointLightEnabled);
        ImGui::Separator();
        for (size_t i = 0; i < kPointLightsAmount; ++i) {
            if (ImGui::TreeNode(fmt::format("Point light {}", i).c_str())) {
                auto& pointLight = scene.GetComponent<elv::PointLightComponent>(m_pointLightEntities[i]);
                auto& pointLightTr = scene.GetComponent<elv::TransformComponent>(m_pointLightEntities[i]);

                if (elv::editor::DrawVec3Control(fmt::format("point_light_pos_{}", i), "Position", pointLightTr.pos)) {
                    pointLightTr.isDirty = true;
                }
                ImGui::Separator();

                ImGui::Text("Point Light Material:");
                elv::editor::DrawRGBColorControl(fmt::format("ambient##point{}", i), pointLight.ambient);
                elv::editor::DrawRGBColorControl(fmt::format("diffuse##point{}", i), pointLight.diffuse);
                elv::editor::DrawRGBColorControl(fmt::format("specular##point{}", i), pointLight.specular);

                ImGui::TreePop();
            }
        }
    }

    if (ImGui::CollapsingHeader("Spot Light")) {
        ImGui::Checkbox("Enable Spot Light", &m_SpotLightEnabled);
        ImGui::Separator();

        auto& flashLight = scene.GetComponent<elv::SpotLightComponent>(m_spotLightEntity);

        elv::editor::DrawSliderFloat("Cut off angle", 0.0f, 180.0f, flashLight.cutOff);
        elv::editor::DrawSliderFloat("Outer cut off angle", 0.0f, 180.0f, flashLight.outerCutOff);
        ImGui::Separator();
        ImGui::Text("Material:");
        elv::editor::DrawRGBColorControl("ambient##spot", flashLight.ambient);
        elv::editor::DrawRGBColorControl("diffuse##spot", flashLight.diffuse);
        elv::editor::DrawRGBColorControl("specular##spot", flashLight.specular);
    }

    ImGui::End();
}
#endif

void MeshModelSandbox::SetEnvironment(const int envIndex)
{
    auto& scene = elv::GetScene();

    const auto& env = kEnvironmenMaterials[envIndex];

    m_clearColor = env.clearColor;

    // directional
    auto& dirLight = scene.GetComponent<elv::DirectionalLightComponent>(m_dirLightEntity);
    auto& dirLightTr = scene.GetComponent<elv::TransformComponent>(m_dirLightEntity);
    dirLight.ambient = env.dirLight.ambient;
    dirLight.diffuse = env.dirLight.diffuse;
    dirLight.specular = env.dirLight.specular;
    dirLightTr.rotation = env.dirLight.direction;

    // flashlight
    auto& flashLight = scene.GetComponent<elv::SpotLightComponent>(m_spotLightEntity);
    flashLight.ambient = env.spotLight.ambient;
    flashLight.diffuse = env.spotLight.diffuse;
    flashLight.specular = env.spotLight.specular;

    // point lights
    for (size_t i = 0; i < kPointLightsAmount; ++i) {
        auto& pointLight = scene.GetComponent<elv::PointLightComponent>(m_pointLightEntities[i]);
        pointLight.ambient = env.pointLightColors[i] * 0.1f;
        pointLight.diffuse = env.pointLightColors[i];
        pointLight.specular = env.pointLightColors[i];
    }
}

void MeshModelSandbox::SetupCubes()
{
    auto& scene = elv::GetScene();

    auto mainCube = scene.CreateEntity();
    m_primitives.emplace_back(mainCube);

    auto& transform = scene.AddComponent<elv::TransformComponent>(mainCube);
    transform.pos = lia::vec3(0.0f, 0.5f, 0.0f);

    auto& mesh = scene.AddComponent<elv::StaticMeshComponent>(mainCube, "cube");
    mesh.AddMaterialTexture(elv::Material::TextureSlot::Diffuse, "wooden_container.png", "assets/images/");
    mesh.AddMaterialTexture(elv::Material::TextureSlot::Specular, "wooden_container_specular.png", "assets/images/");

    for (size_t i = 0; i < kCubesAmount; ++i) {

        auto cube = scene.CreateChildEntity(mainCube);
        m_primitives.emplace_back(cube);

        auto& transform = scene.AddComponent<elv::TransformComponent>(cube, kCubePositions[i], lia::vec3(0.5f));
        auto& childMesh = scene.AddComponent<elv::StaticMeshComponent>(cube, "cube");
        auto& childMaterial = childMesh.GetMaterial();
        childMesh.AddMaterialTexture(elv::Material::TextureSlot::Diffuse, "wooden_container.png", "assets/images/");
        childMesh.AddMaterialTexture(elv::Material::TextureSlot::Specular, "wooden_container_specular.png", "assets/images/");
    }
}
