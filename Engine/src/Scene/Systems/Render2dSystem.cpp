#include "Render2dSystem.h"

#include "Core/Application.h"
#include "Core/Profiler.h"
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

struct SpriteSortingInfo {
    ecs::Entity entity { ecs::INVALID_ENTITY_ID };
    float zOrder { 0.0f };
};

Render2dSystem::Render2dSystem()
    : ecs::IComponentSystem()
{
    m_orthoCameraEntity = Application::Get().GetOrthographicCameraEntity();
}

void Render2dSystem::OnInit()
{
    gFontManager.Load("arial", "assets/fonts/arial.ttf");

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
    if (gEngineSettings.enableZSorting) {
        // current brute solution for the z-order sorting but it hits performance right now
        // TODO: replace this with the Sorting Layers feature
        std::vector<SpriteSortingInfo> sortingInfo;
        const auto& entities = m_spritesPool->GetEntities();
        for (auto entity : entities) {
            sortingInfo.emplace_back(entity, m_trasformsPool->GetComponent(entity).pos.z);
        }

        std::sort(sortingInfo.begin(), sortingInfo.end(), [](const auto& left, const auto& right) {
            return left.zOrder < right.zOrder;
        });

        // drawing
        for (const auto& info : sortingInfo) {
            auto& spriteComponent = m_spritesPool->GetComponent(info.entity);
            auto& transformComponent = m_trasformsPool->GetComponent(info.entity);
            if (spriteComponent.texture != nullptr) {
                elv::Renderer2D::DrawQuad(spriteComponent.texture, transformComponent.pos, transformComponent.scale, transformComponent.rotation.z, spriteComponent.color);
            }
        }
    } else {
        auto& spriteComponents = m_spritesPool->GetComponents();
        for (uint32_t index = 0; index < spriteComponents.size(); ++index) {
            const ecs::Entity entity = m_spritesPool->GetEntity(index);

            auto& spriteComponent = spriteComponents[index];
            auto& transformComponent = m_trasformsPool->GetComponent(entity);
            if (spriteComponent.texture != nullptr) {
                elv::Renderer2D::DrawQuad(spriteComponent.texture, transformComponent.pos, transformComponent.scale, transformComponent.rotation.z, spriteComponent.color);
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
            elv::TextRenderer::RenderText(textComponent.text, textComponent.fontName, rectTransform.pos, rectTransform.scale, textComponent.color);
        }
    }
    //
}
} // namespace elv
