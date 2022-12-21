#pragma once

#include "Scene/ComponentSystem.h"
#include "Scene/Components/SceneComponents.h"

#include "Renderer/OrthographicCameraController.h"

namespace Elven {

class SpriteRenderSystem
    : public ecs::IComponentSystem {
public:
    SpriteRenderSystem(Scene* scenePtr);

    virtual void OnUpdate(float dt) override;
    virtual void OnRender(float dt) override;

private:
    OrthographicCameraController m_cameraController;
};

} // namespace Elven
