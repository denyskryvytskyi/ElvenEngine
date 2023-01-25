#pragma once

#include "Entity.h"

#include <bitset>

namespace elv::ecs {

using ComponentTypeId = std::uint64_t;

constexpr ComponentTypeId INVALID_COMPONENT_TYPE_ID = 0;
constexpr std::uint32_t MAX_ENTITIES_PER_COMPONENT = 100'000;
constexpr std::uint32_t MAX_COMPONENTS = 32;

using ComponentMask = std::bitset<MAX_COMPONENTS>;

class ComponentTypeIdHolder {
public:
    static ComponentTypeId s_componentTypeCounter;
};

template<class ComponentType>
inline ComponentTypeId GetComponentTypeId()
{
    static ComponentTypeId id = INVALID_COMPONENT_TYPE_ID;
    if (id == INVALID_COMPONENT_TYPE_ID) {
        if (ComponentTypeIdHolder::s_componentTypeCounter < MAX_COMPONENTS)
            id = ++ComponentTypeIdHolder::s_componentTypeCounter;
        else {
            EL_CORE_ERROR("Maximum component types limit reached = {0}!", MAX_COMPONENTS);
            EL_CORE_ASSERT(false, "Maximum component types limit reached");
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
        m_components.reserve(MAX_ENTITIES_PER_COMPONENT);
    }

    void Clear() override
    {
        m_components.clear();
        m_entities.clear();
        m_entityToComponentIndex.clear();
    }

    template<typename... Args>
    ComponentType& AddComponent(Entity entity, Args&&... args)
    {
        if (m_components.size() < MAX_ENTITIES_PER_COMPONENT) {
            m_entityToComponentIndex.insert({ entity, m_components.size() });
            m_components.emplace_back(std::forward<Args>(args)...);
            m_entities.emplace_back(entity);
            return m_components.back();
        } else {
            EL_CORE_ASSERT(false, "There isn't component with such entity");
        }

        return m_components.back();
    }

    ComponentType& AddComponent(Entity entity, ComponentType&& component)
    {
        if (m_components.size() < MAX_ENTITIES_PER_COMPONENT) {
            m_entityToComponentIndex.insert({ entity, m_components.size() });
            m_components.emplace_back(std::move(component));
            m_entities.emplace_back(entity);
        } else {
            EL_CORE_ASSERT(false, "There isn't component with such entity");
        }

        return m_components.back();
    }

    void RemoveComponent(Entity entity) override
    {
        auto it = m_entityToComponentIndex.find(entity);
        if (it != m_entityToComponentIndex.end()) {
            const std::uint64_t componentIndex = it->second;

            if (componentIndex < m_components.size() - 1) {
                m_components[componentIndex] = std::move(m_components.back()); // replace dead component with the last one

                Entity movedComponentEntityId = m_entities.back();
                m_entityToComponentIndex[movedComponentEntityId] = componentIndex; // new mapping for moved component
            }

            m_entityToComponentIndex.erase(it);
        }

        if (!m_components.empty()) {
            m_components.pop_back();
            m_entities.pop_back();
        }
    }

    Entity GetEntity(std::uint32_t componentIndex) const override
    {
        return m_entities[componentIndex];
    }

    ComponentType& GetComponent(Entity entity)
    {
        auto it = m_entityToComponentIndex.find(entity);

        if (it == m_entityToComponentIndex.end()) {
            EL_CORE_ASSERT(false, "There isn't component with such entity");
        }

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

private:
    std::vector<ComponentType> m_components;
    std::unordered_map<Entity, std::uint64_t> m_entityToComponentIndex;

    // index - component index that is the same for index from m_components
    // value - entity id associated with this component
    std::vector<Entity> m_entities;
};

} // namespace elv::ecs
