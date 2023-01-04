#pragma once
#include "Entity.h"

namespace Elven {
class Scene;
}

namespace Elven::ecs {
class IBehavior {
public:
    IBehavior();

    virtual ~IBehavior() = default;

    virtual void OnCreate() { }
    virtual void OnDestroy() { }
    virtual void OnUpdate(float dt) { }
    virtual void OnRender(float dt) { }

    void SetEntity(ecs::Entity entity)
    {
        m_entity = entity;
    }

private:
    Entity m_entity { 0 };
    Scene* p_Scene { nullptr };
};
} // namespace Elven::ecs
