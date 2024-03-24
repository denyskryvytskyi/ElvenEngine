#include "PrimitivesSandbox.h"

#include <Renderer/Mesh.h>
#include <Renderer/Renderer.h>
#include <Resources/MeshLibrary.h>
#include <Scene/Components/LightComponent.h>
#include <Scene/Components/StaticMeshComponent.h>

#if EDITOR_MODE
#    include <Editor/EditorHelpers.h>
#    include <imgui.h>
#endif

PrimitivesSandbox::PrimitivesSandbox()
{
    m_cameraController = elv::MakeSharedPtr<elv::EditorCameraController>(45.0f, 1280.0f / 720.0f, 0.1f, 100.0f);

    // scene setup
    auto& scene = elv::GetScene();

    // main cube
    const auto cube = scene.CreateEntity();
    {
        scene.AddComponent<elv::TransformComponent>(cube);
        scene.AddComponent<elv::TagComponent>(cube, "Cube");
        auto& cubeMesh = scene.AddComponent<elv::StaticMeshComponent>(cube, "cube");
        auto cubeMaterial = cubeMesh.GetMaterial();
        cubeMaterial->SetAmbientColor(lia::vec3(0.0f, 1.0f, 1.0f));
        cubeMaterial->SetDiffuseColor(lia::vec3(0.0f, 1.0f, 1.0f));
        cubeMaterial->SetSpecularColor(lia::vec3(0.0f, 1.0f, 1.0f));
        cubeMesh.SetMaterialTexture(elv::Material::TextureSlot::Specular, "white", "");
    }

    // child cube
    {
        const auto childCube = scene.CreateChildEntity(cube);
        scene.AddComponent<elv::TransformComponent>(childCube, lia::vec3(1.5f, 1.0f, 0.0f), lia::vec3(0.5f));
        scene.AddComponent<elv::TagComponent>(childCube, "Cube small");
        auto& cubeMesh = scene.AddComponent<elv::StaticMeshComponent>(childCube, "cube");
        auto cubeMaterial = cubeMesh.GetMaterial();
        cubeMaterial->SetAmbientColor(lia::vec3(0.0f, 1.0f, 1.0f));
        cubeMaterial->SetDiffuseColor(lia::vec3(0.0f, 1.0f, 1.0f));
        cubeMaterial->SetSpecularColor(lia::vec3(0.0f, 1.0f, 1.0f));
        cubeMesh.SetMaterialTexture(elv::Material::TextureSlot::Specular, "white", "");
    }

    // sphere
    const auto sphere = scene.CreateEntity();
    scene.AddComponent<elv::TagComponent>(sphere, "Sphere");
    auto& sphereTransform = scene.AddComponent<elv::TransformComponent>(sphere);
    sphereTransform.pos = lia::vec3(-1.5f, 0.0f, 0.0f);
    auto& sphereMesh = scene.AddComponent<elv::StaticMeshComponent>(sphere, "sphere");
    auto sphereMaterial = sphereMesh.GetMaterial();
    sphereMaterial->SetAmbientColor(lia::vec3(0.5f, 0.5f, 0.5f));
    sphereMaterial->SetDiffuseColor(lia::vec3(0.5f, 0.5f, 0.5f));
    sphereMaterial->SetSpecularColor(lia::vec3(0.5f, 0.5f, 0.5f));
    sphereMesh.SetMaterialTexture(elv::Material::TextureSlot::Specular, "white", "");

    // plane
    const auto plane = scene.CreateEntity();

    scene.AddComponent<elv::TagComponent>(plane, "Plane");
    auto& planeTransform = scene.AddComponent<elv::TransformComponent>(plane);
    planeTransform.pos = lia::vec3(0.0f, -0.51f, 0.0f);
    planeTransform.rotation = lia::vec3(-90.0f, 0.0f, 1.0f);
    planeTransform.scale = lia::vec3(3.0f, 3.0f, 1.0f);
    auto& planeMesh = scene.AddComponent<elv::StaticMeshComponent>(plane, "plane", elv::RenderTopology::TriangleStrip);
    auto planeMaterial = planeMesh.GetMaterial();
    planeMaterial->SetAmbientColor(lia::vec3(1.0f, 1.0f, 0.0f));
    planeMaterial->SetDiffuseColor(lia::vec3(1.0f, 1.0f, 0.0f));
    planeMaterial->SetSpecularColor(lia::vec3(1.0f, 1.0f, 0.0f));
    planeMesh.SetMaterialTexture(elv::Material::TextureSlot::Specular, "white", "");

    // =================== LIGHT =======================
    m_dirLightEntity = scene.CreateEntity();
    scene.AddComponent<elv::TagComponent>(m_dirLightEntity, "Dir light");
    auto& dirLightComponent = scene.AddComponent<elv::DirectionalLightComponent>(m_dirLightEntity);
    dirLightComponent.direction = lia::vec3(0.0f, -1.0f, 0.0f);
    dirLightComponent.enabled = true;

    m_spotLightEntity = scene.CreateEntity();
    scene.AddComponent<elv::TagComponent>(m_spotLightEntity, "Spot light");
    scene.AddComponent<elv::TransformComponent>(m_spotLightEntity);
    auto& spotLightComponent = scene.AddComponent<elv::SpotLightComponent>(m_spotLightEntity);
    spotLightComponent.enabled = true;

    for (size_t i = 0; i < kPointLightsAmount; ++i) {
        m_pointLightEntities[i] = scene.CreateEntity();
        scene.AddComponent<elv::TagComponent>(m_pointLightEntities[i], fmt::format("Point light {}", i));
        scene.AddComponent<elv::TransformComponent>(m_pointLightEntities[i], kPointLightPositions[i]);
        auto& pointLightComponent = scene.AddComponent<elv::PointLightComponent>(m_pointLightEntities[i]);
        pointLightComponent.enabled = true;
    }

    // default environment
    SetEnvironment(0);
}

void PrimitivesSandbox::OnUpdate(float dt)
{
    if (m_cameraController) {
        auto& scene = elv::GetScene();

        auto& flashLightTr = scene.GetComponent<elv::TransformComponent>(m_spotLightEntity);

        flashLightTr.pos = m_cameraController->GetCamera().GetPosition();
        flashLightTr.rotation = m_cameraController->GetFront();
    }
}

void PrimitivesSandbox::OnRender(float dt)
{
}

void PrimitivesSandbox::OnProcessInput(float dt)
{
}

#if EDITOR_MODE
void PrimitivesSandbox::OnImguiRender()
{
    auto& scene = elv::GetScene();

    ImGui::Begin("Environment");

    // environment materials combo
    const char* items[] = { "Default", "Desert", "Factory", "Horror", "Biochemical lab" };
    static int currentEnvironmentIndex = 0;
    const char* combo_label = items[currentEnvironmentIndex];
    if (ImGui::BeginCombo("Preset", combo_label)) {
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

void PrimitivesSandbox::SetEnvironment(const int envIndex)
{
    auto& scene = elv::GetScene();

    const auto& env = kEnvironmenMaterials[envIndex];

    elv::Renderer::SetClearColor(env.clearColor);

    // directional
    auto& dirLight = scene.GetComponent<elv::DirectionalLightComponent>(m_dirLightEntity);
    dirLight.ambient = env.dirLight.ambient;
    dirLight.diffuse = env.dirLight.diffuse;
    dirLight.specular = env.dirLight.specular;
    dirLight.direction = env.dirLight.direction;

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
#endif
