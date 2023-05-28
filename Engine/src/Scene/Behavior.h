#pragma once
#include "Entity.h"
#include "Scene.h"
namespace elv {
class Scene;
}

namespace elv::ecs {
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

    template<class ComponentType>
    ComponentType& GetComponent()
    {
        return p_Scene->GetComponent<ComponentType>(m_entity);
    }

protected:
    Entity m_entity { 0 };
    elv::Scene* p_Scene { nullptr };
};
} // namespace elv::ecs
