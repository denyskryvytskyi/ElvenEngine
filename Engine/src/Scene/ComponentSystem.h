#pragma once

#include "Component.h"
#include "Entity.h"

namespace Elven {
class Scene;
}

namespace Elven::ecs {

class IComponentSystem {
public:
    virtual ~IComponentSystem() = default;

    virtual void OnUpdate(float dt) { }
    virtual void OnRender(float dt) { }
};

template<typename... ComponentTypes>
class ComponentSystem : public IComponentSystem {
public:
    ComponentSystem(Scene* scenePtr)
        : m_pScene(scenePtr)
    {
    }

    virtual ~ComponentSystem() = default;

protected:
    Scene* m_pScene { nullptr };
};
} // namespace Elven::ecs
