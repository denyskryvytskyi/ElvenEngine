#pragma once

#include "Component.h"
#include "Entity.h"

namespace Elven::ecs {

class World {
public:
    World() = default;

    EntityId CreateEntity()
    {
        const EntityId id = CreateEntityId();
        m_entities.insert({ id, ComponentMask() });

        return id;
    }

    const std::unordered_map<EntityId, ComponentMask>& GetEntities() const
    {
        return m_entities;
    }

    void DestroyEntity(const EntityId& entityId)
    {
        auto entityIt = m_entities.find(entityId);

        if (entityIt != m_entities.end()) {
            auto componentMask = entityIt->second;
            for (size_t i = 0; i < componentMask.size(); ++i) {
                if (componentMask.test(i)) {
                    m_componentPools[i]->RemoveComponent(entityId);
                }
            }
            m_entities.erase(entityId);
        }
    }

    void OnShutdown()
    {
        m_componentPools.clear();
        m_entities.clear();
    }

    template<typename ComponentType>
    void AddComponent(EntityId entityId)
    {
        auto entityIt = m_entities.find(entityId);

        const ComponentTypeId componentTypeId = GetComponentTypeId<ComponentType>();
        entityIt->second.set(componentTypeId);

        auto componentArrayIt = m_componentPools.find(componentTypeId);
        if (componentArrayIt != m_componentPools.end()) {
            componentArrayIt->second->AddComponent(entityId);
        } else {
            m_componentPools.insert({ componentTypeId, MakeSharedPtr<ComponentPool<ComponentType>>() });
            m_componentPools[componentTypeId]->AddComponent(entityId);
        }
    };

    template<typename ComponentType>
    void RemoveComponent(EntityId entityId)
    {
        auto entityIt = m_entities.find(entityId);

        const ComponentTypeId componentTypeId = GetComponentTypeId<ComponentType>();
        entityIt->second.set(componentTypeId, false);

        auto componentArrayIt = m_componentPools.find(componentTypeId);
        if (componentArrayIt != m_componentPools.end()) {
            componentArrayIt->second->RemoveComponent(entityId);
        } else {
            EL_CORE_ASSERT(false, "Component type isn't registered.")
        }
    };

    template<typename ComponentType>
    bool HasComponent(EntityId entityId)
    {
        auto entityIt = m_entities.find(entityId);

        if (entityIt != m_entities.end()) {
            const ComponentTypeId componentTypeId = GetComponentTypeId<ComponentType>();
            return entityIt->second.test(componentTypeId);
        }

        return false;
    }

    template<typename ComponentType>
    ComponentType& GetComponent(EntityId entityId)
    {
        const ComponentTypeId componentTypeId = GetComponentTypeId<ComponentType>();
        auto it = m_componentPools.find(componentTypeId);

        if (it == m_componentPools.end()) {
            EL_CORE_ASSERT(false, "Component type isn't registered.")
        }

        auto test = std::static_pointer_cast<ComponentPool<ComponentType>>(it->second);
        return std::static_pointer_cast<ComponentPool<ComponentType>>(it->second)->GetComponent(entityId);
    }

    template<typename ComponentType>
    const std::vector<ComponentType>& GetComponents() const
    {
        const ComponentTypeId componentTypeId = GetComponentTypeId<ComponentType>();
        auto it = m_componentPools.find(componentTypeId);

        if (it == m_componentPools.end()) {
            EL_CORE_ASSERT(false, "Component type isn't registered.")
        }

        return std::static_pointer_cast<ComponentPool<ComponentType>>(it->second)->GetComponents();
    }
    template<typename ComponentType>
    bool HasComponents() const
    {
        const ComponentTypeId componentTypeId = GetComponentTypeId<ComponentType>();
        auto it = m_componentPools.find(componentTypeId);

        return it != m_componentPools.end();
    }

    template<typename ComponentType>
    EntityId GetEntity(std::uint32_t componentIndex) const
    {
        const ComponentTypeId componentTypeId = GetComponentTypeId<ComponentType>();
        auto it = m_componentPools.find(componentTypeId);

        if (it == m_componentPools.end()) {
            EL_CORE_ASSERT(false, "Component type isn't registered.")
        }

        return it->second->GetEntity(componentIndex);
    }

private:
    std::unordered_map<ComponentTypeId, SharedPtr<ComponentPoolBase>> m_componentPools;
    std::unordered_map<EntityId, ComponentMask> m_entities;
};

} // namespace Elven::ecs
