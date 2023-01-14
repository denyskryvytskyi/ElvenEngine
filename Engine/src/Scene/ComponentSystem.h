#pragma once

#include "Component.h"
#include "Entity.h"

namespace elv {
class Scene;
}

namespace elv::ecs {

class IComponentSystem {
public:
    IComponentSystem(Scene* scenePtr)
        : m_pScene(scenePtr)
    {
    }

    virtual ~IComponentSystem() = default;

    virtual void OnInit() { }
    virtual void OnShutdown() { }
    virtual void OnUpdate(float dt) { }
    virtual void OnRender(float dt) { }

protected:
    Scene* m_pScene { nullptr };
};

} // namespace elv::ecs
