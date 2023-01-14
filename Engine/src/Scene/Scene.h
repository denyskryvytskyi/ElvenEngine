#pragma once

#include "Component.h"
#include "ComponentSystem.h"
#include "Entity.h"

namespace elv {

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
        m_entitiesSignatures.insert({ id, ecs::ComponentMask() });
        m_entities.emplace_back(id);

        return id;
    }

    const std::vector<ecs::Entity>& GetEntities() const
    {
        return m_entities;
    }

    void DestroyEntity(const ecs::Entity& entity)
    {
        auto entityIt = std::find(m_entities.begin(), m_entities.end(), entity);

        if (entityIt != m_entities.end()) {
            auto componentMask = m_entitiesSignatures[entity];
            for (size_t i = 0; i < componentMask.size(); ++i) {
                if (componentMask.test(i)) {
                    m_componentPools[i]->RemoveComponent(entity);
                }
            }
            m_entitiesSignatures.erase(entity);
            m_entities.erase(entityIt);
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

    template<typename ComponentType, typename... Args>
    ComponentType& AddComponent(ecs::Entity entity, Args&&... args)
    {
        auto entityIt = m_entitiesSignatures.find(entity);
        const ecs::ComponentTypeId componentTypeId = ecs::GetComponentTypeId<ComponentType>();
        entityIt->second.set(componentTypeId);

        auto componentPoolIt = m_componentPools.find(componentTypeId);
        EL_CORE_ASSERT(componentPoolIt != m_componentPools.end(), "Component pool isn't registered");

        return std::static_pointer_cast<ecs::ComponentPool<ComponentType>>(componentPoolIt->second)->AddComponent(entity, std::forward<Args>(args)...);
    };

    template<typename ComponentType>
    ComponentType& AddComponent(ecs::Entity entity, ComponentType&& component)
    {
        auto entityIt = m_entitiesSignatures.find(entity);
        const ecs::ComponentTypeId componentTypeId = ecs::GetComponentTypeId<ComponentType>();
        entityIt->second.set(componentTypeId);

        auto componentPoolIt = m_componentPools.find(componentTypeId);

        EL_CORE_ASSERT(componentPoolIt != m_componentPools.end(), "Component pool isn't registered");

        return std::static_pointer_cast<ecs::ComponentPool<ComponentType>>(componentPoolIt->second)->AddComponent(entity, std::forward<ComponentType>(component));
    };

    template<typename ComponentType>
    void RemoveComponent(ecs::Entity entity)
    {
        auto entityIt = m_entitiesSignatures.find(entity);
        const ecs::ComponentTypeId componentTypeId = ecs::GetComponentTypeId<ComponentType>();
        entityIt->second.set(componentTypeId, false);

        auto componentPoolIt = m_componentPools.find(componentTypeId);

        EL_CORE_ASSERT(componentPoolIt != m_componentPools.end(), "Component type isn't registered.")

        componentPoolIt->second->RemoveComponent(entity);
    };

    template<typename ComponentType>
    bool HasComponent(ecs::Entity entity)
    {
        auto entityIt = m_entitiesSignatures.find(entity);

        if (entityIt != m_entitiesSignatures.end()) {
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

        EL_CORE_ASSERT(it != m_componentPools.end(), "Component type isn't registered.");

        return std::static_pointer_cast<ecs::ComponentPool<ComponentType>>(it->second)->GetComponent(entity);
    }

    template<typename ComponentType>
    std::vector<ComponentType>& GetComponents() const
    {
        const ecs::ComponentTypeId componentTypeId = ecs::GetComponentTypeId<ComponentType>();
        auto it = m_componentPools.find(componentTypeId);

        EL_CORE_ASSERT(it != m_componentPools.end(), "Component type isn't registered.")

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

        EL_CORE_ASSERT(it != m_componentPools.end(), "Component type isn't registered.")

        return it->second->GetEntity(componentIndex);
    }

    template<typename SystemType>
    void RegisterSystem()
    {
        m_systems.emplace_back(MakeUniquePtr<SystemType>(this));
        m_systems.back()->OnInit();
    }

private:
    SceneNodeComponent root;

    std::unordered_map<ecs::ComponentTypeId, SharedPtr<ecs::IComponentPool>> m_componentPools;
    std::vector<UniquePtr<ecs::IComponentSystem>> m_systems;
    std::vector<ecs::Entity> m_entities;
    std::unordered_map<ecs::Entity, ecs::ComponentMask> m_entitiesSignatures;
};

} // namespace elv
