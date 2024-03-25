#include "LightSystem.h"

#include "Scene/Components/LightComponent.h"
#include "Scene/Components/TransformComponent.h"
#include "Scene/Scene.h"

#include "Core/Application.h"
#include "Renderer/CameraController.h"

namespace elv {
void LightSystem::OnUpdate(float dt)
{
    auto spotLightsPool = m_pScene->GetComponentPool<SpotLightComponent>();
    auto& spotLightComponents = spotLightsPool->GetComponents();
    for (std::uint32_t i = 0; i < spotLightsPool->Size(); ++i) {
        auto& spotlight = spotLightComponents[i];
        if (!spotlight.enabled)
            continue;

        if (spotlight.flashlightMode) {
            const auto entity = spotLightsPool->GetEntity(i);
            if (m_pScene->HasComponent<TransformComponent>(entity)) {
                auto& transform = m_pScene->GetComponent<TransformComponent>(entity);

                const auto cameraController = Application::Get().GetCameraController();

                transform.pos = cameraController->GetCamera().GetPosition();
                transform.rotation = cameraController->GetFront();
                break;
            }
        }
    }
}
} // namespace elv
