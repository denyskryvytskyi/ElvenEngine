#pragma once

#include "Component.h"
#include "Entity.h"

namespace Elven::ecs {

class World {
public:
    World()
    {
        m_entites.reserve(MAX_ENTITIES_PER_COMPONENT * MAX_COMPONENTS);
    }

    EntityId CreateEntity()
    {
        const EntityId id = CreateEntityId();
        m_entites.emplace_back(id, ComponentMask());

        return id;
    }

    const std::vector<Entity>& GetEntities() const
    {
        return m_entites;
    }

    void DestroyEntity(const EntityId& entityId)
    {
        auto entityIt = std::find_if(m_entites.begin(), m_entites.end(), [&entityId](const Entity& entity) {
            return entity.id == entityId;
        });

        if (entityIt != m_entites.end()) {
            auto componentMask = entityIt->componentMask;
            for (size_t i = 0; i < componentMask.size(); ++i) {
                if (componentMask.test(i)) {
                    m_componentArrays[i]->RemoveComponent(entityIt->id);
                }
            }
            m_entites.erase(entityIt);
        }
    }

    void OnShutdown()
    {
        m_componentArrays.clear();
        m_entites.clear();
    }

    template<typename ComponentType>
    void AddComponent(EntityId entityId)
    {
        auto entityIt = std::find_if(m_entites.begin(), m_entites.end(), [&entityId](const Entity& entity) {
            return entity.id == entityId;
        });

        const ComponentTypeId componentTypeId = GetComponentTypeId<ComponentType>();
        entityIt->componentMask.set(componentTypeId);

        auto componentArrayIt = m_componentArrays.find(componentTypeId);
        if (componentArrayIt != m_componentArrays.end()) {
            componentArrayIt->second->AddComponent(entityId);
        } else {
            m_componentArrays.insert({ componentTypeId, MakeSharedPtr<ComponentArray<ComponentType>>() });
            m_componentArrays[componentTypeId]->AddComponent(entityId);
        }
    };

    template<typename ComponentType>
    void RemoveComponent(EntityId entityId)
    {
        auto entityIt = std::find_if(m_entites.begin(), m_entites.end(), [&entityId](const Entity& entity) {
            return entity.id == entityId;
        });

        const ComponentTypeId componentTypeId = GetComponentTypeId<ComponentType>();
        entityIt->componentMask.set(componentTypeId, false);

        auto componentArrayIt = m_componentArrays.find(componentTypeId);
        if (componentArrayIt != m_componentArrays.end()) {
            componentArrayIt->second->RemoveComponent(entityId);
        } else {
            EL_CORE_ASSERT(false, "Component type isn't registered.")
        }
    };

    template<typename ComponentType>
    bool HasComponent(EntityId entityId)
    {
        auto entityIt = std::find_if(m_entites.begin(), m_entites.end(), [&entityId](const Entity& entity) {
            return entity.id == entityId;
        });

        if (entityIt != m_entites.end()) {
            const ComponentTypeId componentTypeId = GetComponentTypeId<ComponentType>();
            return entityIt->componentMask.test(componentTypeId);
        }

        return false;
    }

    template<typename ComponentType>
    ComponentType& GetComponent(EntityId entityId)
    {
        const ComponentTypeId componentTypeId = GetComponentTypeId<ComponentType>();
        auto it = m_componentArrays.find(componentTypeId);

        if (it == m_componentArrays.end()) {
            EL_CORE_ASSERT(false, "Component type isn't registered.")
        }

        auto test = std::static_pointer_cast<ComponentArray<ComponentType>>(it->second);
        return std::static_pointer_cast<ComponentArray<ComponentType>>(it->second)->GetComponent(entityId);
    }

    template<typename ComponentType>
    const std::vector<ComponentType>& GetComponents() const
    {
        const ComponentTypeId componentTypeId = GetComponentTypeId<ComponentType>();
        auto it = m_componentArrays.find(componentTypeId);

        if (it == m_componentArrays.end()) {
            EL_CORE_ASSERT(false, "Component type isn't registered.")
        }

        return std::static_pointer_cast<ComponentArray<ComponentType>>(it->second)->GetComponents();
    }

private:
    std::unordered_map<ComponentTypeId, SharedPtr<ComponentArrayBase>> m_componentArrays;
    std::vector<Entity> m_entites;
};

} // namespace Elven::ecs
