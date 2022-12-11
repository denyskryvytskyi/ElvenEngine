#pragma once

#include "Component.h"
#include "Entity.h"

namespace Elven {
class Scene;
}

namespace Elven::ecs {

class IComponentSystem {
public:
    IComponentSystem(Scene* scenePtr)
        : m_pScene(scenePtr)
    {
    }
    virtual ~IComponentSystem() = default;

    virtual void OnUpdate(float dt) { }
    virtual void OnRender(float dt) { }

protected:
    Scene* m_pScene { nullptr };
};

} // namespace Elven::ecs
