#pragma once

#include "Component.h"
#include "Entity.h"

namespace Elven {
class Scene;
}

namespace Elven::ecs {

class ComponentSystemInterface {
public:
    virtual ~ComponentSystemInterface() = default;

    virtual void OnUpdate(float dt) { }
    virtual void OnRender(float dt) { }
};

template<typename... ComponentTypes>
class ComponentSystem : public ComponentSystemInterface {
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
