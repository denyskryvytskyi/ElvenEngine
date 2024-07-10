#pragma once

#include "Core/Core.h"
#include "Core/Log.h"
#include "Entity.h"

#include <bitset>

namespace elv::ecs {

using ComponentTypeId = std::uint64_t;

inline constexpr ComponentTypeId INVALID_COMPONENT_TYPE_ID = 0;
inline constexpr std::uint32_t INIT_COMPONENT_POOL_CAPACITY = 100'000;
inline constexpr std::uint32_t MAX_COMPONENT_TYPES = 64;

using ComponentMask = std::bitset<MAX_COMPONENT_TYPES>;

class ComponentTypeIdHolder {
public:
    static ComponentTypeId s_componentTypeCounter;
};

template<class ComponentType>
inline ComponentTypeId GetComponentTypeId()
{
    static ComponentTypeId id = INVALID_COMPONENT_TYPE_ID;
    if (id == INVALID_COMPONENT_TYPE_ID) {
        if (ComponentTypeIdHolder::s_componentTypeCounter < MAX_COMPONENT_TYPES)
            id = ++ComponentTypeIdHolder::s_componentTypeCounter;
        else {
            EL_CORE_ERROR("Maximum component types limit reached = {0}!", MAX_COMPONENT_TYPES);
            EL_CORE_ASSERT(false, "Maximum component types limit reached");
            return INVALID_COMPONENT_TYPE_ID;
        }
    }

    return id;
}

struct IComponentPool {
    virtual ~IComponentPool() = default;

    virtual void Clear() = 0;
    virtual void RemoveComponent(Entity entity) = 0;
    virtual Entity GetEntity(std::uint32_t componentIndex) const = 0;
};

template<class ComponentType>
class ComponentPool final : public IComponentPool {
public:
    ComponentPool()
    {
        m_components.reserve(INIT_COMPONENT_POOL_CAPACITY);
    }

    void Clear() override
    {
        m_components.clear();
        m_entities.clear();
        m_entityToComponentIndex.clear();
    }

    template<typename... Args>
    ComponentType& AddComponent(Entity entity)
    {
        m_entityToComponentIndex.insert({ entity, m_components.size() });
        m_components.emplace_back();
        m_entities.emplace_back(entity);
        return m_components.back();
    }

    template<typename... Args>
    ComponentType& AddComponent(Entity entity, Args&&... args)
    {
        m_entityToComponentIndex.insert({ entity, m_components.size() });
        ComponentType component { std::forward<Args>(args)... };
        m_components.push_back(component);
        m_entities.emplace_back(entity);
        return m_components.back();
    }

    ComponentType& AddComponent(Entity entity, ComponentType&& component)
    {
        m_entityToComponentIndex.insert({ entity, m_components.size() });
        m_components.emplace_back(std::move(component));
        m_entities.emplace_back(entity);
        return m_components.back();
    }

    void RemoveComponent(Entity entity) override
    {
        auto it = m_entityToComponentIndex.find(entity);
        EL_CORE_ASSERT(it != m_entityToComponentIndex.end(), "Entity has not component of this type");

        const std::uint64_t componentIndex = it->second;

        if (componentIndex < m_components.size() - 1) {
            // replace dead component with the last one
            m_components[componentIndex] = std::move(m_components.back());
            m_entities[componentIndex] = std::move(m_entities.back());

            const Entity movedComponentEntityId = m_entities.back();
            m_entityToComponentIndex[movedComponentEntityId] = componentIndex; // new mapping for moved component
        }

        m_entityToComponentIndex.erase(it);

        if (!m_components.empty()) {
            m_components.pop_back();
            m_entities.pop_back();
        }
    }

    Entity GetEntity(std::uint32_t componentIndex) const override
    {
        return m_entities[componentIndex];
    }

    ComponentType& GetByIndex(const std::uint32_t componentIndex)
    {
        EL_CORE_ASSERT(m_components.size() > componentIndex, "Out of components pool range.")
        return m_components[componentIndex];
    }

    ComponentType& GetComponent(Entity entity)
    {
        auto it = m_entityToComponentIndex.find(entity);

        EL_CORE_ASSERT(it != m_entityToComponentIndex.end(), "The entity hasn't component of this type");

        return m_components[it->second];
    }

    std::vector<ComponentType>& GetComponents()
    {
        return m_components;
    }

    const std::vector<ComponentType>& GetComponents() const
    {
        return m_components;
    }

    const std::vector<Entity>& GetEntities() const
    {
        return m_entities;
    }

    size_t Size() const
    {
        return m_components.size();
    }

private:
    std::vector<ComponentType> m_components;
    std::unordered_map<Entity, std::uint64_t> m_entityToComponentIndex;

    // index - component index that is the same for index from m_components
    // value - entity id associated with this component
    std::vector<Entity> m_entities;
};

} // namespace elv::ecs
