#include "Render2dSystem.h"

#include "Core/SettingsConfig.h"
#include "Events/EventManager.h"
#include "Events/TextureEvent.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/TextureManager.h"
#include "Scene/Scene.h"

namespace elv {
Render2dSystem::Render2dSystem(Scene* scenePtr)
    : ecs::IComponentSystem(scenePtr)
    , m_cameraController(static_cast<float>(gEngineSettings.WindowWidth) / static_cast<float>(gEngineSettings.WindowHeight))
{
}

void Render2dSystem::OnUpdate(float dt)
{
    m_cameraController.OnUpdate(dt);
}

void Render2dSystem::OnRender(float dt)
{
    Renderer2D::BeginScene(m_cameraController.GetCamera());

    // Sprites
    auto& spriteComponents = m_pScene->GetComponents<SpriteComponent>();
    for (uint32_t index = 0; index < spriteComponents.size(); ++index) {
        const ecs::Entity entity = m_pScene->GetEntity<SpriteComponent>(index);

        auto& spriteComponent = spriteComponents[index];
        auto& transformComponent = m_pScene->GetComponent<TransformComponent>(entity);
        if (spriteComponent.texture != nullptr) {
            elv::Renderer2D::DrawQuad(spriteComponent.texture, transformComponent.pos, transformComponent.scale, transformComponent.rotation.z);
        }
    }
    //

    // Quads
    auto& quadComponents = m_pScene->GetComponents<QuadComponent>();
    for (uint32_t index = 0; index < quadComponents.size(); ++index) {
        const ecs::Entity entity = m_pScene->GetEntity<QuadComponent>(index);

        auto& quadComponent = quadComponents[index];
        auto& transformComponent = m_pScene->GetComponent<TransformComponent>(entity);
        elv::Renderer2D::DrawQuad(transformComponent.pos, transformComponent.scale, transformComponent.rotation.z, quadComponent.color);
    }
    //

    Renderer2D::EndScene();
}

} // namespace elv
