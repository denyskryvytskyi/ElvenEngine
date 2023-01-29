#include "Render2dSystem.h"

#include "Core/Application.h"
#include "Core/SettingsConfig.h"
#include "Events/EventManager.h"
#include "Events/TextureEvent.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/TextRenderer.h"
#include "Renderer/TextureManager.h"
#include "Scene/Scene.h"

namespace elv {
Render2dSystem::Render2dSystem(Scene* scenePtr)
    : ecs::IComponentSystem(scenePtr)
{
    m_orthoCameraEntity = Application::Get().GetOrthographicCameraEntity();
}

void Render2dSystem::OnInit()
{
    elv::TextRenderer::Load("assets/fonts/arial.ttf");
}

void Render2dSystem::OnUpdate(float dt)
{
}

void Render2dSystem::OnRender(float dt)
{
    auto& camera = m_pScene->GetComponent<CameraComponent>(m_orthoCameraEntity).camera;
    Renderer2D::BeginScene(camera);

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

    // Text
    TextRenderer::PreRender(camera);
    elv::TextRenderer::RenderText("Hello there! +-(!@#$%^&s*(-_dslgnp! I Did it :)", { 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f, 0.0f, 1.0f });
    //
}
} // namespace elv
