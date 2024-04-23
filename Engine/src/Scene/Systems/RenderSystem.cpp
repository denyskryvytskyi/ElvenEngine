#include "RenderSystem.h"

#include "Core/Application.h"
#include "Core/Profiler.h"
#include "Renderer/CameraController.h"
#include "Renderer/Mesh.h"
#include "Renderer/RHI/Shader.h"
#include "Renderer/Renderer.h"
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
    auto& renderer = Application::Get().GetRenderer();

    renderer.EnableFaceCulling(true);

    auto cameraController = Application::Get().GetCameraController();
    if (!cameraController) {
        EL_CORE_ERROR("Failed to find main camera");
        return;
    }
    auto& camera = cameraController->GetCamera();

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
        int activePointLightsCounter = 0;

        auto pointLightsPool = m_pScene->GetComponentPool<PointLightComponent>();
        auto& pointLightComponents = pointLightsPool->GetComponents();
        for (std::uint32_t i = 0; i < pointLightsPool->Size(); ++i) {
            auto& pointLight = pointLightComponents.at(i);
            if (!pointLight.enabled)
                continue;

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
        m_shader->SetInteger("u_ActivePointLightsAmount", activePointLightsCounter);
    }

    // spotlights
    {
        int activeSpotLightsCounter = 0;
        auto spotLightsPool = m_pScene->GetComponentPool<SpotLightComponent>();
        auto& spotLightComponents = spotLightsPool->GetComponents();
        for (std::uint32_t i = 0; i < spotLightsPool->Size(); ++i) {
            auto& spotlight = spotLightComponents[i];
            if (!spotlight.enabled)
                continue;

            const auto entity = spotLightsPool->GetEntity(i);
            if (m_pScene->HasComponent<TransformComponent>(entity)) {
                auto& transform = m_pScene->GetComponent<TransformComponent>(entity);

                m_shader->SetVector3f(fmt::format("u_SpotLights[{}].position", activeSpotLightsCounter), transform.pos);
                m_shader->SetVector3f(fmt::format("u_SpotLights[{}].direction", activeSpotLightsCounter), transform.rotation);
                m_shader->SetVector3f(fmt::format("u_SpotLights[{}].ambient", activeSpotLightsCounter), spotlight.ambient);
                m_shader->SetVector3f(fmt::format("u_SpotLights[{}].diffuse", activeSpotLightsCounter), spotlight.diffuse);
                m_shader->SetVector3f(fmt::format("u_SpotLights[{}].specular", activeSpotLightsCounter), spotlight.specular);
                m_shader->SetFloat(fmt::format("u_SpotLights[{}].cutOff", activeSpotLightsCounter), cos(lia::radians(spotlight.cutOff)));
                m_shader->SetFloat(fmt::format("u_SpotLights[{}].outerCutOff", activeSpotLightsCounter), cos(lia::radians(spotlight.outerCutOff)));
                m_shader->SetFloat(fmt::format("u_SpotLights[{}].constant", activeSpotLightsCounter), spotlight.constant);
                m_shader->SetFloat(fmt::format("u_SpotLights[{}].linear", activeSpotLightsCounter), spotlight.linear);
                m_shader->SetFloat(fmt::format("u_SpotLights[{}].quadratic", activeSpotLightsCounter), spotlight.quadratic);
                ++activeSpotLightsCounter;
            }
        }
        m_shader->SetInteger("u_ActiveSpotLightsAmount", activeSpotLightsCounter);
    }

    // ===================================================

    // TODO: Need to implement sorting logic for proper rendering of transparent objects

    // render static mesh
    {
        PROFILE_SCOPE("Static Meshes rendered in: ");
        auto meshPool = m_pScene->GetComponentPool<StaticMeshComponent>();
        auto& meshComponents = meshPool->GetComponents();
        for (std::uint32_t i = 0; i < meshPool->Size(); ++i) {
            auto entity = meshPool->GetEntity(i);

            if (m_pScene->HasComponent<TransformComponent>(entity)) {
                auto& meshComponent = meshComponents[i];
                auto& transform = m_pScene->GetComponent<TransformComponent>(entity);

                m_shader->SetMatrix4("u_NormalModel", transform.normalMatrix);
                const auto& meshPtr = meshComponent.GetMeshPtr();
                if (meshPtr) {
                    renderer.Submit(m_shader, meshPtr, transform.modelMatrix);
                }
            }
        }
    }

    // render debug point/spot lights meshes
    const bool renderDebugPointLightsMesh = true;
    if (renderDebugPointLightsMesh) {
        m_lightShader->Bind();

        // point lights
        auto pointLightsPool = m_pScene->GetComponentPool<PointLightComponent>();
        auto& pointLightComponents = pointLightsPool->GetComponents();
        for (std::uint32_t i = 0; i < pointLightsPool->Size(); ++i) {
            auto& pointLight = pointLightComponents.at(i);
            if (!pointLight.enabled) {
                continue;
            }

            if (pointLight.debugRender) {
                const auto entity = pointLightsPool->GetEntity(i);
                if (m_pScene->HasComponent<TransformComponent>(entity)) {
                    auto& transform = m_pScene->GetComponent<TransformComponent>(entity);

                    m_lightShader->SetVector3f("u_Color.ambient", pointLight.ambient);
                    m_lightShader->SetVector3f("u_Color.diffuse", pointLight.diffuse);

                    renderer.Submit(m_lightShader, m_debugLightMesh, transform.modelMatrix);
                }
            }
        }

        // spotlights
        auto spotLightsPool = m_pScene->GetComponentPool<SpotLightComponent>();
        auto& spotLightComponents = spotLightsPool->GetComponents();
        for (std::uint32_t i = 0; i < spotLightsPool->Size(); ++i) {
            auto& spotlight = spotLightComponents[i];
            if (spotlight.debugRender) {
                const auto entity = spotLightsPool->GetEntity(i);
                if (m_pScene->HasComponent<TransformComponent>(entity)) {
                    auto& transform = m_pScene->GetComponent<TransformComponent>(entity);

                    m_lightShader->SetVector3f("u_Color.ambient", spotlight.ambient);
                    m_lightShader->SetVector3f("u_Color.diffuse", spotlight.diffuse);

                    renderer.Submit(m_lightShader, m_debugLightMesh, transform.modelMatrix);
                }
            }
        }
    }

    renderer.EnableFaceCulling(false);
}
} // namespace elv
