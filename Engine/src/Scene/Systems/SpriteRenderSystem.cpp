#include "SpriteRenderSystem.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"
#include "Scene/Scene.h"

namespace Elven {
SpriteRenderSystem::SpriteRenderSystem(Scene* scenePtr)
    : ComponentSystem(scenePtr)
    , m_cameraController(1280.0f / 720.0f)
{
}

void SpriteRenderSystem::OnUpdate(float dt)
{
    m_cameraController.OnUpdate(dt);
}

void SpriteRenderSystem::OnRender(float dt)
{
    Elven::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
    Elven::RenderCommand::Clear();

    Elven::Renderer2D::BeginScene(m_cameraController.GetCamera());

    auto& transformComponents = m_pScene->GetComponents<TransformComponent>();
    for (std::uint32_t index = 0; index < transformComponents.size(); ++index) {
        const ecs::Entity entity = m_pScene->GetEntity<TransformComponent>(index);

        auto& transformComponent = transformComponents[index];
        if (m_pScene->HasComponent<SpriteComponent>(entity)) {
            auto& spriteComponent = m_pScene->GetComponent<SpriteComponent>(entity);
            Elven::Renderer2D::DrawQuad(spriteComponent.m_texture, transformComponent.pos, transformComponent.scale, 45.0f);
        } else {
            Elven::Renderer2D::DrawQuad(transformComponent.pos, transformComponent.scale, 0.0f, { 0.5f, 0.5f, 0.2f, 1.0f });
        }
    }

    Elven::Renderer2D::EndScene();
}
} // namespace Elven
