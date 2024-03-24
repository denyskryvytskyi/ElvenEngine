#include "RenderSystem.h"

#include "Core/Application.h"
#include "Renderer/CameraController.h"
#include "Renderer/Mesh.h"
#include "Renderer/Renderer.h"
#include "Renderer/Shader.h"
#include "Resources/MeshLibrary.h"
#include "Scene/Components/LightComponent.h"
#include "Scene/Components/StaticMeshComponent.h"
#include "Scene/Components/TransformComponent.h"
#include "Scene/Scene.h"

namespace elv {
void RenderSystem::OnInit()
{
    m_shader = ShaderManager::Load("mesh_phong", "mesh_phong.vert", "mesh_phong.frag");
    m_debugLightMesh = gMeshLibrary.GetMesh("sphere");
    m_lightShader = ShaderManager::Load("light_debug", "light_debug.vert", "light_debug.frag");
}

void RenderSystem::OnRender(float dt)
{
    auto cameraController = Application::Get().GetCameraController();
    if (!cameraController) {
        EL_CORE_ERROR("Failed to find main camera");
        return;
    }
    auto& camera = cameraController->GetCamera();

    Renderer::BeginScene(camera);

    m_shader->Bind();
    m_shader->SetVector3f("u_ViewPos", camera.GetPosition());

    // ================== LIGHT UNIFORMS ===========================

    // directional light
    auto& dirLights = m_pScene->GetComponents<DirectionalLightComponent>();
    if (dirLights.size() > 0) {
        auto& dirLight = dirLights.at(0);
        if (dirLight.enabled) {
            m_shader->SetVector3f("u_DirLight.direction", dirLight.direction);
            m_shader->SetVector3f("u_DirLight.ambient", dirLight.ambient);
            m_shader->SetVector3f("u_DirLight.diffuse", dirLight.diffuse);
            m_shader->SetVector3f("u_DirLight.specular", dirLight.specular);
        }
        m_shader->SetInteger("u_DirLightEnabled", dirLight.enabled);
    }

    // point lights
    {
        bool isAnyPointLightActive = false;
        int activePointLightsCounter = 0;

        auto pointLightsPool = m_pScene->GetComponentPool<PointLightComponent>();
        auto& pointLightComponents = pointLightsPool->GetComponents();
        for (std::uint32_t i = 0; i < pointLightsPool->Size(); ++i) {
            auto& pointLight = pointLightComponents.at(i);
            if (!pointLight.enabled)
                continue;

            isAnyPointLightActive = true;
            const auto entity = pointLightsPool->GetEntity(i);
            if (m_pScene->HasComponent<TransformComponent>(entity)) {
                auto& transform = m_pScene->GetComponent<TransformComponent>(entity);

                m_shader->SetVector3f(fmt::format("u_PointLights[{}].position", activePointLightsCounter), transform.pos);
                m_shader->SetVector3f(fmt::format("u_PointLights[{}].ambient", activePointLightsCounter), pointLight.ambient);
                m_shader->SetVector3f(fmt::format("u_PointLights[{}].diffuse", activePointLightsCounter), pointLight.diffuse);
                m_shader->SetVector3f(fmt::format("u_PointLights[{}].specular", activePointLightsCounter), pointLight.specular);
                m_shader->SetFloat(fmt::format("u_PointLights[{}].constant", activePointLightsCounter), pointLight.constant);
                m_shader->SetFloat(fmt::format("u_PointLights[{}].linear", activePointLightsCounter), pointLight.linear);
                m_shader->SetFloat(fmt::format("u_PointLights[{}].quadratic", activePointLightsCounter), pointLight.quadratic);
                ++activePointLightsCounter;
            }
        }
        m_shader->SetInteger("u_PointLightEnabled", isAnyPointLightActive);
        m_shader->SetInteger("u_ActivePointLightsAmount", activePointLightsCounter);
        // TODO: need to pass how much point lights are activated
    }

    // spotlight (flashlight in this example with camera position and view direction)
    bool isAnySpotlightAvailable = false;
    auto spotLightsPool = m_pScene->GetComponentPool<SpotLightComponent>();
    auto& spotLightComponents = spotLightsPool->GetComponents();
    for (std::uint32_t i = 0; i < spotLightsPool->Size(); ++i) {
        auto& spotlight = spotLightComponents[i];
        if (!spotlight.enabled)
            continue;

        isAnySpotlightAvailable = true;

        const auto entity = spotLightsPool->GetEntity(i);
        if (m_pScene->HasComponent<TransformComponent>(entity)) {
            auto& transform = m_pScene->GetComponent<TransformComponent>(entity);

            m_shader->SetVector3f("u_SpotLight.position", transform.pos);
            m_shader->SetVector3f("u_SpotLight.direction", transform.rotation);
            m_shader->SetVector3f("u_SpotLight.ambient", spotlight.ambient);
            m_shader->SetVector3f("u_SpotLight.diffuse", spotlight.diffuse);
            m_shader->SetVector3f("u_SpotLight.specular", spotlight.specular);
            m_shader->SetFloat("u_SpotLight.cutOff", cos(lia::radians(spotlight.cutOff)));
            m_shader->SetFloat("u_SpotLight.outerCutOff", cos(lia::radians(spotlight.outerCutOff)));
            m_shader->SetFloat("u_SpotLight.constant", spotlight.constant);
            m_shader->SetFloat("u_SpotLight.linear", spotlight.linear);
            m_shader->SetFloat("u_SpotLight.quadratic", spotlight.quadratic);
        }
    }
    m_shader->SetInteger("u_SpotLightEnabled", isAnySpotlightAvailable);

    // ===================================================

    // TODO: Need to implement sorting logic for proper rendering of transparent objects

    // render static mesh
    auto meshPool = m_pScene->GetComponentPool<StaticMeshComponent>();
    auto& meshComponents = meshPool->GetComponents();
    for (std::uint32_t i = 0; i < meshPool->Size(); ++i) {
        auto entity = meshPool->GetEntity(i);

        if (m_pScene->HasComponent<TransformComponent>(entity)) {
            auto& meshComponent = meshComponents[i];
            auto& transform = m_pScene->GetComponent<TransformComponent>(entity);

            // TODO: maybe cache inverse world matrix too
            m_shader->SetMatrix4("u_InversedNormalModel", lia::inverse(transform.worldMatrix));
            const auto& meshPtr = meshComponent.GetMeshPtr();
            if (meshPtr) {
                Renderer::Submit(m_shader, meshPtr, transform.worldMatrix);
            }
        }
    }

    // render debug render point lights meshes
    const bool renderDebugPointLightsMesh = true;
    if (renderDebugPointLightsMesh) {
        m_lightShader->Bind();
        auto pointLightsPool = m_pScene->GetComponentPool<PointLightComponent>();
        auto& pointLightComponents = pointLightsPool->GetComponents();
        for (std::uint32_t i = 0; i < pointLightsPool->Size(); ++i) {
            auto& pointLight = pointLightComponents.at(i);
            if (!pointLight.enabled) {
                continue;
            }

            const auto entity = pointLightsPool->GetEntity(i);
            if (m_pScene->HasComponent<TransformComponent>(entity)) {
                auto& transform = m_pScene->GetComponent<TransformComponent>(entity);

                m_lightShader->SetVector3f("u_Color.ambient", pointLight.ambient);
                m_lightShader->SetVector3f("u_Color.diffuse", pointLight.diffuse);

                Renderer::Submit(m_lightShader, m_debugLightMesh, transform.worldMatrix);
            }
        }
    }

    Renderer::EndScene();
}
} // namespace elv
