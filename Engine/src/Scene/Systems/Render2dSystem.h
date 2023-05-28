#pragma once

#include "Scene/ComponentSystem.h"
#include "Scene/Components/SceneComponents.h"

#include "Renderer/OrthographicCameraController.h"

namespace elv {

class Render2dSystem
    : public ecs::IComponentSystem {
public:
    Render2dSystem();

    void OnInit() override;
    void OnRender(float dt) override;

private:
    ecs::Entity m_orthoCameraEntity { ecs::INVALID_ENTITY_ID };

    SharedPtr<ecs::ComponentPool<SpriteComponent>> m_spritesPool { nullptr };
    SharedPtr<ecs::ComponentPool<TransformComponent>> m_trasformsPool { nullptr };
    SharedPtr<ecs::ComponentPool<QuadComponent>> m_quadsPool { nullptr };
    SharedPtr<ecs::ComponentPool<TextComponent>> m_textsPool { nullptr };
    SharedPtr<ecs::ComponentPool<RectTransformComponent>> m_rectTransformPool { nullptr };
};

} // namespace elv
