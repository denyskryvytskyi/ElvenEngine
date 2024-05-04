#include "Render2dSystem.h"

#include "Core/Application.h"
#include "Core/Profiler.h"
#include "Core/SettingsConfig.h"
#include "Events/EventManager.h"
#include "Events/TextureEvent.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/TextRenderer.h"
#include "Resources/FontManager.h"
#include "Resources/TextureManager.h"
#include "Scene/Scene.h"

namespace elv {

struct SpriteSortingInfo {
    ecs::Entity entity { ecs::INVALID_ENTITY_ID };
    float zOrder { 0.0f };
};

void Render2dSystem::OnInit()
{
    gFontManager.Load("arial", "assets/fonts/arial.ttf");

    m_spritesPool = m_pScene->GetComponentPool<SpriteComponent>();
    m_trasformsPool = m_pScene->GetComponentPool<TransformComponent>();
    m_quadsPool = m_pScene->GetComponentPool<QuadComponent>();
    m_textsPool = m_pScene->GetComponentPool<TextComponent>();
    m_rectTransformPool = m_pScene->GetComponentPool<UITransformComponent>();
}

void Render2dSystem::OnRender(float dt)
{
    const ecs::Entity orthoCameraEntity = Application::Get().GetOrthographicCameraEntity();
    auto& camera = m_pScene->GetComponent<CameraComponent>(orthoCameraEntity).camera;
    Renderer2D::BeginScene(camera);

    // Sprites
    if (gEngineSettings.enableZSorting) {
        // current brute solution for the z-order sorting but it hits performance right now
        // TODO: replace this with the Sorting Layers feature
        std::vector<SpriteSortingInfo> sortingInfo;
        const auto& entities = m_spritesPool->GetEntities();
        for (auto entity : entities) {
            SpriteSortingInfo info;
            info.entity = entity;
            info.zOrder = m_trasformsPool->GetComponent(entity).GetPosition().z;
            sortingInfo.push_back(info);
        }

        std::sort(sortingInfo.begin(), sortingInfo.end(), [](const auto& left, const auto& right) {
            return left.zOrder < right.zOrder;
        });

        // drawing
        for (const auto& info : sortingInfo) {
            auto& spriteComponent = m_spritesPool->GetComponent(info.entity);
            auto& transformComponent = m_trasformsPool->GetComponent(info.entity);
            if (spriteComponent.texture != nullptr) {
                Renderer2D::DrawQuad(spriteComponent.texture, transformComponent.GetModelMatrix(), spriteComponent.color);
            }
        }
    } else {
        auto& spriteComponents = m_spritesPool->GetComponents();
        for (uint32_t index = 0; index < spriteComponents.size(); ++index) {
            const ecs::Entity entity = m_spritesPool->GetEntity(index);

            if (m_pScene->HasComponent<TransformComponent>(entity)) {
                auto& spriteComponent = spriteComponents[index];
                auto& transformComponent = m_trasformsPool->GetComponent(entity);
                if (spriteComponent.texture != nullptr) {
                    Renderer2D::DrawQuad(spriteComponent.texture, transformComponent.GetModelMatrix(), spriteComponent.color);
                }
            }
        }
    }

    //

    // Quads
    auto& quadComponents = m_quadsPool->GetComponents();
    for (uint32_t index = 0; index < quadComponents.size(); ++index) {
        const ecs::Entity entity = m_quadsPool->GetEntity(index);

        auto& quadComponent = quadComponents[index];
        auto& transformComponent = m_trasformsPool->GetComponent(entity);
        Renderer2D::DrawQuad(transformComponent.GetModelMatrix(), quadComponent.color);
    }
    //
    Renderer2D::EndScene();

    // Text
    auto& renderer = Application::Get().GetRenderer();
    renderer.EnableDepthTesting(false);

    TextRenderer::PreRender(camera);
    auto& textComponents = m_textsPool->GetComponents();
    for (uint32_t i = 0; i < textComponents.size(); ++i) {
        const ecs::Entity entity = m_textsPool->GetEntity(i);

        auto& textComponent = textComponents[i];
        if (textComponent.isVisible) {

            if (m_pScene->HasComponent<UITransformComponent>(entity)) {
                auto& rectTransform = m_rectTransformPool->GetComponent(entity);
                TextRenderer::RenderText(renderer, textComponent.text, textComponent.fontName, rectTransform.pos, rectTransform.scale, textComponent.color);
            }
        }
    }
    renderer.EnableDepthTesting(true);
}
} // namespace elv
