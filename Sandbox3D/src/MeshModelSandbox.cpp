#include "MeshModelSandbox.h"

#include <Resources/AudioManager.h>
#include <Scene/Components/LightComponent.h>
#include <Scene/Components/StaticMeshComponent.h>

#if EDITOR_MODE
#    include <Editor/EditorHelpers.h>
#    include <imgui.h>
#endif

const int kCubesAmount = 1;

MeshModelSandbox::MeshModelSandbox()
{
    m_cameraController = elv::MakeSharedPtr<elv::EditorCameraController>(45.0f, 1280.0f / 720.0f, 0.1f, 1000.0f);

    // models
    auto& scene = elv::GetScene();

    if (true) {
        const auto knights = scene.CreateEntity();
        scene.AddComponent<elv::TagComponent>(knights, "knights");
        for (size_t i = 0; i < 8; i++) {
            const auto knight = scene.CreateChildEntity(knights);
            scene.AddComponent<elv::TagComponent>(knights, fmt::format("knight_{}", i));
            scene.AddComponent<elv::TransformComponent>(knight, lia::vec3(3.0f + i, 0.0f, -2.0f), lia::vec3(0.01f));
            scene.AddComponent<elv::StaticMeshComponent>(knight, "knight", fmt::format("{}{}", elv::fileSystem::MODELS_PATH, "dark_knight/scene.gltf"));
        }
    }

    if (true) {
        const auto walle = scene.CreateEntity();
        scene.AddComponent<elv::TagComponent>(walle, "Walle");
        scene.AddComponent<elv::TransformComponent>(walle, lia::vec3(0.0f, 0.0f, 0.0f), lia::vec3(1.0f));
        scene.AddComponent<elv::StaticMeshComponent>(walle, "walle", fmt::format("{}{}", elv::fileSystem::MODELS_PATH, "walle/walle.obj"));
    }

    if (false) {
        const auto backpack = scene.CreateEntity();
        scene.AddComponent<elv::TagComponent>(backpack, "Backpack");
        scene.AddComponent<elv::TransformComponent>(backpack, lia::vec3(3.0f, 0.0f, 0.0f), lia::vec3(0.01f));
        scene.AddComponent<elv::StaticMeshComponent>(backpack, "backpack", fmt::format("{}{}", elv::fileSystem::MODELS_PATH, "backpack_gltf/scene.gltf"));
    }

    if (false) {
        const auto robot = scene.CreateEntity();
        scene.AddComponent<elv::TagComponent>(robot, "Robot");
        scene.AddComponent<elv::TransformComponent>(robot, lia::vec3(20.0f, 0.0f, 0.0f), lia::vec3(1.0f));
        scene.AddComponent<elv::StaticMeshComponent>(robot, "robot", fmt::format("{}{}", elv::fileSystem::MODELS_PATH, "robot/robot.obj"));
    }

    const auto entity = scene.CreateEntity();
    scene.AddComponent<elv::TagComponent>(entity, "Cerberus");
    scene.AddComponent<elv::TransformComponent>(entity, lia::vec3(-2.0f, 2.0f, 0.0f), lia::vec3(0.01f));
    auto& cerberusMesh = scene.AddComponent<elv::StaticMeshComponent>(entity, "cerberus", fmt::format("{}{}", elv::fileSystem::MODELS_PATH, "cerberus/cerberus.fbx"));
    cerberusMesh.SetMaterialTexture(elv::Material::TextureSlot::Specular, "Cerberus_M.tga", fmt::format("{}{}", elv::fileSystem::MODELS_PATH, "cerberus/Textures"));

    const auto sponza = scene.CreateEntity();
    scene.AddComponent<elv::TagComponent>(sponza, "Sponza");
    scene.AddComponent<elv::TransformComponent>(sponza, lia::vec3(0.0f), lia::vec3(0.01f));
    scene.AddComponent<elv::StaticMeshComponent>(sponza, "sponza", fmt::format("{}{}", elv::fileSystem::MODELS_PATH, "sponza/sponza.obj"));

    // cubes
    SetupCubes();

    // sphere
    const auto sphere = scene.CreateEntity();
    scene.AddComponent<elv::TagComponent>(sphere, "Sphere");

    auto& transform = scene.AddComponent<elv::TransformComponent>(sphere);
    transform.pos = lia::vec3(-2.0f, 0.5f, 0.0f);
    auto& sphereMesh = scene.AddComponent<elv::StaticMeshComponent>(sphere, "sphere");
    auto sphereMaterial = sphereMesh.GetMaterial();
    sphereMaterial->SetAmbientColor(lia::vec3(0.0f, 1.0f, 0.0f));
    sphereMaterial->SetDiffuseColor(lia::vec3(0.0f, 1.0f, 0.0f));
    sphereMaterial->SetSpecularColor(lia::vec3(0.0f, 1.0f, 0.0f));
    sphereMesh.SetMaterialTexture(elv::Material::TextureSlot::Specular, "white", "");
    //  sphereMesh.SetMaterialTexture(elv::Material::TextureSlot::Diffuse, "sphere.jpg", "assets/images");

    // =================== LIGHT =======================
    auto dirLightEntity = scene.CreateEntity();
    scene.AddComponent<elv::TagComponent>(dirLightEntity, "Directional light");
    auto& dirLightComponent = scene.AddComponent<elv::DirectionalLightComponent>(dirLightEntity);
    dirLightComponent.direction = lia::vec3(0.0f, -1.0f, 0.0f);
    dirLightComponent.enabled = true;

    auto flashLightEntity = scene.CreateEntity();
    scene.AddComponent<elv::TagComponent>(flashLightEntity, "Spot light");
    scene.AddComponent<elv::TransformComponent>(flashLightEntity);
    auto& spotLightComponent = scene.AddComponent<elv::SpotLightComponent>(flashLightEntity);
    spotLightComponent.enabled = true;

    for (size_t i = 0; i < kPointLightsAmount; ++i) {
        auto pointLightEntity = scene.CreateEntity();
        scene.AddComponent<elv::TagComponent>(pointLightEntity, fmt::format("Point light {}", i));
        scene.AddComponent<elv::TransformComponent>(pointLightEntity, kPointLightPositions[i]);
        auto& pointLightComponent = scene.AddComponent<elv::PointLightComponent>(pointLightEntity);
        pointLightComponent.enabled = true;
    }

    // default environment
    SetEnvironment(0);

    // audio
    elv::gAudioManager.AddSound("background music", "back.flac");
    elv::gAudioManager.AddSound("test", "test.wav");
}

void MeshModelSandbox::OnUpdate(float dt)
{
}

void MeshModelSandbox::OnRender(float dt)
{
}

void MeshModelSandbox::OnProcessInput(float dt)
{
}

#if EDITOR_MODE
void MeshModelSandbox::OnImguiRender()
{
    ImGui::Begin("Environment");

    // environment materials combo
    const char* items[] = { "Default", "Desert", "Factory", "Horror", "Biochemical lab" };
    static int currentEnvironmentIndex = 0;
    const char* combo_label = items[currentEnvironmentIndex];
    if (ImGui::BeginCombo("Preset ", combo_label)) {
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

    ImGui::End();
}
#endif

void MeshModelSandbox::SetEnvironment(const int envIndex)
{
    auto& scene = elv::GetScene();

    const auto& env = kEnvironmenMaterials[envIndex];

    elv::Renderer::SetClearColor(env.clearColor);

    // directional
    auto& dirLights = scene.GetComponents<elv::DirectionalLightComponent>();
    if (dirLights.size() > 0) {
        dirLights[0].ambient = env.dirLight.ambient;
        dirLights[0].diffuse = env.dirLight.diffuse;
        dirLights[0].specular = env.dirLight.specular;
        dirLights[0].direction = env.dirLight.direction;
    }

    // spotlights
    for (auto& spotlight : scene.GetComponents<elv::SpotLightComponent>()) {
        spotlight.ambient = env.spotLight.ambient;
        spotlight.diffuse = env.spotLight.diffuse;
        spotlight.specular = env.spotLight.specular;
    }

    // point lights
    auto& pointLights = scene.GetComponents<elv::PointLightComponent>();
    for (size_t i = 0; i < kPointLightsAmount && i < pointLights.size(); ++i) {
        pointLights[i].ambient = env.pointLightColors[i] * 0.1f;
        pointLights[i].diffuse = env.pointLightColors[i];
        pointLights[i].specular = env.pointLightColors[i];
    }
}

void MeshModelSandbox::SetupCubes()
{
    auto& scene = elv::GetScene();

    auto mainCube = scene.CreateEntity();
    scene.AddComponent<elv::TagComponent>(mainCube, "Cube");

    auto& transform = scene.AddComponent<elv::TransformComponent>(mainCube);
    transform.pos = lia::vec3(0.0f, 0.5f, 0.0f);

    auto& mesh = scene.AddComponent<elv::StaticMeshComponent>(mainCube, "cube");
    mesh.SetMaterialTexture(elv::Material::TextureSlot::Diffuse, "wooden_container.png", "assets/images/");
    mesh.SetMaterialTexture(elv::Material::TextureSlot::Specular, "wooden_container_specular.png", "assets/images/");

    for (size_t i = 0; i < kCubesAmount; ++i) {

        auto cube = scene.CreateChildEntity(mainCube);
        scene.AddComponent<elv::TagComponent>(cube, "Small cube");

        auto& transform = scene.AddComponent<elv::TransformComponent>(cube, kCubePositions[i], lia::vec3(0.5f));
        auto& childMesh = scene.AddComponent<elv::StaticMeshComponent>(cube, "cube");
        childMesh.SetMaterialTexture(elv::Material::TextureSlot::Diffuse, "wooden_container.png", "assets/images/");
        childMesh.SetMaterialTexture(elv::Material::TextureSlot::Specular, "wooden_container_specular.png", "assets/images/");
    }
}
