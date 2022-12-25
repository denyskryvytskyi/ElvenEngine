#include "SpriteRenderSystem.h"

#include "Core/SettingsConfig.h"
#include "Events/EventManager.h"
#include "Events/TextureEvent.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/TextureManager.h"
#include "Scene/Scene.h"

namespace Elven {
SpriteRenderSystem::SpriteRenderSystem(Scene* scenePtr)
    : ecs::IComponentSystem(scenePtr)
    , m_cameraController(static_cast<float>(gEngineSettings.WindowWidth) / static_cast<float>(gEngineSettings.WindowHeight))
{
}

void SpriteRenderSystem::OnUpdate(float dt)
{
    m_cameraController.OnUpdate(dt);
}

void SpriteRenderSystem::OnRender(float dt)
{
    Elven::Renderer2D::BeginScene(m_cameraController.GetCamera());

    auto& transformComponents = m_pScene->GetComponents<TransformComponent>();
    for (std::uint32_t index = 0; index < transformComponents.size(); ++index) {
        const ecs::Entity entity = m_pScene->GetEntity<TransformComponent>(index);

        auto& transformComponent = transformComponents[index];
        if (m_pScene->HasComponent<SpriteComponent>(entity)) {
            auto& spriteComponent = m_pScene->GetComponent<SpriteComponent>(entity);
            if (spriteComponent.texture != nullptr) {
                Elven::Renderer2D::DrawQuad(spriteComponent.texture, transformComponent.pos, transformComponent.scale, 45.0f);
            }
        } else {
            Elven::Renderer2D::DrawQuad(transformComponent.pos, transformComponent.scale, 0.0f, { 0.5f, 0.5f, 0.2f, 1.0f });
        }
    }

    Elven::Renderer2D::EndScene();
}
} // namespace Elven
