#pragma once

#include "Scene/ComponentSystem.h"
#include "Scene/Components/SceneComponents.h"

#include "Renderer/OrthographicCameraController.h"

namespace elv {

class Render2dSystem
    : public ecs::IComponentSystem {
public:
    Render2dSystem(Scene* scenePtr);

    virtual void OnUpdate(float dt) override;
    virtual void OnRender(float dt) override;

private:
    ecs::Entity m_orthoCameraEntity { ecs::INVALID_ENTITY_ID };
};

} // namespace elv
