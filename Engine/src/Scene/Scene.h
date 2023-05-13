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
        const ecs::Entity id = ecs::GenerateEntityId();
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
            const auto& componentMask = entityIt->second;
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
        auto it = m_componentPools.find(componentTypeId);
        if (it == m_componentPools.end()) {
            m_componentPools.insert({ componentTypeId, MakeSharedPtr<ecs::ComponentPool<ComponentType>>() });
        } else {
            EL_CORE_WARN("Component type is already registered with id = {0}", componentTypeId);
        }
    }

    template<typename ComponentType, typename... Args>
    ComponentType& AddComponent(ecs::Entity entity, Args&&... args)
    {
        const ecs::ComponentTypeId componentTypeId = ecs::GetComponentTypeId<ComponentType>();

        auto it = m_componentPools.find(componentTypeId);
        EL_CORE_ASSERT(it != m_componentPools.end(), "Failed to add component. Component type isn't registered.");

        auto entityIt = m_entities.find(entity);
        EL_CORE_ASSERT(entityIt != m_entities.end(), "The entity isn't found.");
        entityIt->second.set(componentTypeId);

        return std::static_pointer_cast<ecs::ComponentPool<ComponentType>>(it->second)->AddComponent(entity, std::forward<Args>(args)...);
    };

    template<typename ComponentType>
    ComponentType& AddComponent(ecs::Entity entity, ComponentType&& component)
    {
        const ecs::ComponentTypeId componentTypeId = ecs::GetComponentTypeId<ComponentType>();

        auto it = m_componentPools.find(componentTypeId);
        EL_CORE_ASSERT(it != m_componentPools.end(), "Failed to add component. Component type isn't registered.");

        auto entityIt = m_entities.find(entity);
        EL_CORE_ASSERT(entityIt != m_entities.end(), "The entity isn't found.");
        entityIt->second.set(componentTypeId);

        return std::static_pointer_cast<ecs::ComponentPool<ComponentType>>(it->second)->AddComponent(entity, std::forward<ComponentType>(component));
    };

    template<typename ComponentType>
    void RemoveComponent(ecs::Entity entity)
    {
        auto entityIt = m_entities.find(entity);
        EL_CORE_ASSERT(entityIt != m_entities.end(), "The entity isn't found.");

        const ecs::ComponentTypeId componentTypeId = ecs::GetComponentTypeId<ComponentType>();
        auto it = m_componentPools.find(componentTypeId);
        EL_CORE_ASSERT(it != m_componentPools.end(), "Failed to remove component. Component type isn't registered");

        entityIt->second.set(componentTypeId, false);
        it->second->RemoveComponent(entity);
    };

    template<typename ComponentType>
    bool HasComponent(ecs::Entity entity)
    {
        auto entityIt = m_entities.find(entity);
        EL_CORE_ASSERT(entityIt != m_entities.end(), "The entity isn't found");

        const ecs::ComponentTypeId componentTypeId = ecs::GetComponentTypeId<ComponentType>();
        return entityIt->second.test(componentTypeId);
    }

    template<typename ComponentType>
    ComponentType& GetComponent(ecs::Entity entity)
    {
        auto entityIt = m_entities.find(entity);
        EL_CORE_ASSERT(entityIt != m_entities.end(), "The entity isn't found");

        const ecs::ComponentTypeId componentTypeId = ecs::GetComponentTypeId<ComponentType>();
        EL_CORE_ASSERT(entityIt->second.test(componentTypeId), "The entity hasn't component of this type.")

        auto it = m_componentPools.find(componentTypeId);
        EL_CORE_ASSERT(it != m_componentPools.end(), "Failed to get component. Component type isn't registered.")

        return std::static_pointer_cast<ecs::ComponentPool<ComponentType>>(it->second)->GetComponent(entity);
    }

    template<typename ComponentType>
    std::vector<ComponentType>& GetComponents() const
    {
        const ecs::ComponentTypeId componentTypeId = ecs::GetComponentTypeId<ComponentType>();
        auto it = m_componentPools.find(componentTypeId);
        EL_CORE_ASSERT(it != m_componentPools.end(), "Failed to get components. Component type isn't registered.");

        return std::static_pointer_cast<ecs::ComponentPool<ComponentType>>(it->second)->GetComponents();
    }

    template<typename ComponentType>
    bool IsComponentTypeRegistered() const
    {
        const ecs::ComponentTypeId componentTypeId = ecs::GetComponentTypeId<ComponentType>();
        auto it = m_componentPools.find(componentTypeId);

        return it != m_componentPools.end();
    }

    template<typename ComponentType>
    SharedPtr<ecs::ComponentPool<ComponentType>> GetComponentPool()
    {
        const ecs::ComponentTypeId componentTypeId = ecs::GetComponentTypeId<ComponentType>();
        auto it = m_componentPools.find(componentTypeId);
        EL_CORE_ASSERT(it != m_componentPools.end(), "Failed to get component pool. Component type isn't registered.");

        return std::static_pointer_cast<ecs::ComponentPool<ComponentType>>(it->second);
    }

    template<typename ComponentType>
    ecs::Entity GetEntity(std::uint32_t componentIndex) const
    {
        const ecs::ComponentTypeId componentTypeId = ecs::GetComponentTypeId<ComponentType>();
        auto it = m_componentPools.find(componentTypeId);
        EL_CORE_ASSERT(it != m_componentPools.end(), "Failed to get entity by component index. Component type isn't registered.");

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
    std::unordered_map<ecs::Entity, ecs::ComponentMask> m_entities;
};

} // namespace elv
