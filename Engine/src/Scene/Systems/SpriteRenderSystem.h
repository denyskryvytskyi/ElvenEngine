#pragma once

#include "Scene/ComponentSystem.h"
#include "Scene/Components/ComponentsDef.h"

#include "Renderer/OrthographicCameraController.h"

namespace Elven {

class SpriteRenderSystem
    : public ecs::ComponentSystem<TransformComponent, SpriteComponent> {
public:
    SpriteRenderSystem(Scene* scenePtr);

    virtual void OnUpdate(float dt) override;
    virtual void OnRender(float dt) override;

private:
    Elven::OrthographicCameraController m_cameraController;
};

} // namespace Elven
