#pragma once

#include "Component.h"
#include "ComponentSystem.h"
#include "Entity.h"

namespace Elven {

struct SceneNodeComponent {
    std::vector<UniquePtr<SceneNodeComponent>> m_childs;
    ecs::Entity m_entityId;
};

class Scene {
public:
    Scene();

    void OnInit();
    void OnShutdown();
    void OnUpdate(float dt);
    void OnRender(float dt);

    //------------- ECS INTERFACE -------------------
    ecs::Entity CreateEntity()
    {
        const ecs::Entity id = ecs::CreateEntity();
        m_entities.insert({ id, ecs::ComponentMask() });

        return id;
    }

    const std::unordered_map<ecs::Entity, ecs::ComponentMask>& GetEntities() const
    {
        return m_entities;
    }

    void DestroyEntity(const ecs::Entity& entity)
    {
        auto entityIt = m_entities.find(entity);

        if (entityIt != m_entities.end()) {
            auto componentMask = entityIt->second;
            for (size_t i = 0; i < componentMask.size(); ++i) {
                if (componentMask.test(i)) {
                    m_componentPools[i]->RemoveComponent(entity);
                }
            }
            m_entities.erase(entity);
        }
    }

    template<typename ComponentType>
    void RegisterComponent()
    {
        const ecs::ComponentTypeId componentTypeId = ecs::GetComponentTypeId<ComponentType>();
        auto componentPoolIt = m_componentPools.find(componentTypeId);
        if (componentPoolIt == m_componentPools.end()) {
            m_componentPools.insert({ componentTypeId, MakeSharedPtr<ecs::ComponentPool<ComponentType>>() });
        } else {
            EL_CORE_WARN("Component pool already registered, id = {0}", componentTypeId);
        }
    }

    template<typename ComponentType>
    void AddComponent(ecs::Entity entity)
    {
        auto entityIt = m_entities.find(entity);
        const ecs::ComponentTypeId componentTypeId = ecs::GetComponentTypeId<ComponentType>();
        entityIt->second.set(componentTypeId);

        auto componentPoolIt = m_componentPools.find(componentTypeId);
        if (componentPoolIt != m_componentPools.end()) {
            componentPoolIt->second->AddComponent(entity);
        } else {
            EL_CORE_ASSERT("Component pool isn't registered");
        }
    };

    template<typename ComponentType>
    void RemoveComponent(ecs::Entity entity)
    {
        auto entityIt = m_entities.find(entity);
        const ecs::ComponentTypeId componentTypeId = ecs::GetComponentTypeId<ComponentType>();
        entityIt->second.set(componentTypeId, false);

        auto componentPoolIt = m_componentPools.find(componentTypeId);
        if (componentPoolIt != m_componentPools.end()) {
            componentPoolIt->second->RemoveComponent(entity);
        } else {
            EL_CORE_ASSERT(false, "Component type isn't registered.")
        }
    };

    template<typename ComponentType>
    bool HasComponent(ecs::Entity entity)
    {
        auto entityIt = m_entities.find(entity);

        if (entityIt != m_entities.end()) {
            const ecs::ComponentTypeId componentTypeId = ecs::GetComponentTypeId<ComponentType>();
            return entityIt->second.test(componentTypeId);
        }

        return false;
    }

    template<typename ComponentType>
    ComponentType& GetComponent(ecs::Entity entity)
    {
        const ecs::ComponentTypeId componentTypeId = ecs::GetComponentTypeId<ComponentType>();
        auto it = m_componentPools.find(componentTypeId);

        if (it == m_componentPools.end()) {
            EL_CORE_ASSERT(false, "Component type isn't registered.");
        }

        return std::static_pointer_cast<ecs::ComponentPool<ComponentType>>(it->second)->GetComponent(entity);
    }

    template<typename ComponentType>
    const std::vector<ComponentType>& GetComponents() const
    {
        const ecs::ComponentTypeId componentTypeId = ecs::GetComponentTypeId<ComponentType>();
        auto it = m_componentPools.find(componentTypeId);

        if (it == m_componentPools.end()) {
            EL_CORE_ASSERT(false, "Component type isn't registered.")
        }

        return std::static_pointer_cast<ecs::ComponentPool<ComponentType>>(it->second)->GetComponents();
    }
    template<typename ComponentType>
    bool HasComponentPool() const
    {
        const ecs::ComponentTypeId componentTypeId = ecs::GetComponentTypeId<ComponentType>();
        auto it = m_componentPools.find(componentTypeId);

        return it != m_componentPools.end();
    }

    template<typename ComponentType>
    ecs::Entity GetEntity(std::uint32_t componentIndex) const
    {
        const ecs::ComponentTypeId componentTypeId = ecs::GetComponentTypeId<ComponentType>();
        auto it = m_componentPools.find(componentTypeId);

        if (it == m_componentPools.end()) {
            EL_CORE_ASSERT(false, "Component type isn't registered.")
        }

        return it->second->GetEntity(componentIndex);
    }

    template<typename SystemType>
    void RegisterSystem()
    {
        m_systems.emplace_back(MakeUniquePtr<SystemType>(this));
    }

private:
    SceneNodeComponent root;

    std::unordered_map<ecs::ComponentTypeId, SharedPtr<ecs::IComponentPool>> m_componentPools;
    std::unordered_map<ecs::Entity, ecs::ComponentMask> m_entities;
    std::vector<UniquePtr<ecs::IComponentSystem>> m_systems;
};

} // namespace Elven
