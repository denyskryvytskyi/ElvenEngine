#pragma once

#include "Scene/ComponentSystem.h"
#include "Scene/Components/SceneComponents.h"

namespace Elven {
class Primitives2dDrawSystem
    : public ecs::IComponentSystem {
public:
    Primitives2dDrawSystem(Scene* scenePtr);

    virtual void OnUpdate(float dt) override;
    virtual void OnRender(float dt) override;

private:
};
} // namespace Elven
