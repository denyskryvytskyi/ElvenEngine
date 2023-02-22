#include "Render2dSystem.h"

#include "Core/Application.h"
#include "Core/SettingsConfig.h"
#include "Events/EventManager.h"
#include "Events/TextureEvent.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/TextRenderer.h"
#include "Resources/FontManager.h"
#include "Resources/TextureManager.h"
#include "Scene/Scene.h"

namespace elv {
Render2dSystem::Render2dSystem(Scene* scenePtr)
    : ecs::IComponentSystem(scenePtr)
{
    m_orthoCameraEntity = Application::Get().GetOrthographicCameraEntity();
}

void Render2dSystem::OnInit()
{
    gFontManager.Load("assets/fonts/arial.ttf");

    m_spritesPool = m_pScene->GetComponentPool<SpriteComponent>();
    m_trasformsPool = m_pScene->GetComponentPool<TransformComponent>();
    m_quadsPool = m_pScene->GetComponentPool<QuadComponent>();
    m_textsPool = m_pScene->GetComponentPool<TextComponent>();
    m_rectTransformPool = m_pScene->GetComponentPool<RectTransformComponent>();
}

void Render2dSystem::OnRender(float dt)
{
    auto& camera = m_pScene->GetComponent<CameraComponent>(m_orthoCameraEntity).camera;
    Renderer2D::BeginScene(camera);

    // Sprites
    auto& spriteComponents = m_spritesPool->GetComponents();
    for (uint32_t index = 0; index < spriteComponents.size(); ++index) {
        const ecs::Entity entity = m_spritesPool->GetEntity(index);

        auto& spriteComponent = spriteComponents[index];
        auto& transformComponent = m_trasformsPool->GetComponent(entity);
        if (spriteComponent.texture != nullptr) {
            elv::Renderer2D::DrawQuad(spriteComponent.texture, transformComponent.pos, transformComponent.scale, transformComponent.rotation.z);
        }
    }
    //

    // Quads
    auto& quadComponents = m_quadsPool->GetComponents();
    for (uint32_t index = 0; index < quadComponents.size(); ++index) {
        const ecs::Entity entity = m_quadsPool->GetEntity(index);

        auto& quadComponent = quadComponents[index];
        auto& transformComponent = m_trasformsPool->GetComponent(entity);
        elv::Renderer2D::DrawQuad(transformComponent.pos, transformComponent.scale, transformComponent.rotation.z, quadComponent.color);
    }
    //
    Renderer2D::EndScene();

    // Text
    TextRenderer::PreRender(camera);
    auto& textComponents = m_textsPool->GetComponents();
    for (uint32_t i = 0; i < textComponents.size(); ++i) {
        const ecs::Entity entity = m_textsPool->GetEntity(i);

        auto& textComponent = textComponents[i];
        if (textComponent.isVisible) {
            auto& rectTransform = m_rectTransformPool->GetComponent(entity);
            elv::TextRenderer::RenderText(textComponent.text, rectTransform.pos, rectTransform.scale, textComponent.color);
        }
    }
    //
}
} // namespace elv
