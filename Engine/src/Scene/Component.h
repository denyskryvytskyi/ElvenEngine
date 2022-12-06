#pragma once

#include "Entity.h"

#include <bitset>

namespace Elven::ecs {

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
            EL_CORE_ERROR("Maximum component types limet reached = {0}!", MAX_COMPONENTS);
            EL_ASSERT(false);
        }
    }

    return id;
}

struct ComponentPoolBase {
    virtual ~ComponentPoolBase() = default;

    virtual void Clear() = 0;
    virtual void AddComponent(Entity entity) = 0;
    virtual void RemoveComponent(Entity entity) = 0;
    virtual Entity GetEntity(std::uint32_t componentIndex) const = 0;
};

template<class ComponentType>
class ComponentPool final : public ComponentPoolBase {
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

    void AddComponent(Entity entity) override
    {
        if (m_components.size() < MAX_ENTITIES_PER_COMPONENT) {
            m_entityToComponentIndex.insert({ entity, m_components.size() });
            m_components.emplace_back();
            m_entities.emplace_back(entity);
        } else {
            EL_CORE_CRITICAL("Max components limit reached.");
        }
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
        }

        m_components.pop_back();
        m_entities.pop_back();
        m_entityToComponentIndex.erase(it);
    }

    Entity GetEntity(std::uint32_t componentIndex) const override
    {
        return m_entities[componentIndex];
    }

    ComponentType& GetComponent(Entity entity)
    {
        auto it = m_entityToComponentIndex.find(entity);

        if (it == m_entityToComponentIndex.end()) {
            EL_CORE_CRITICAL("There isn't component with entity id = {0}", entity);
            EL_CORE_ASSERT(false);
        }

        return m_components[it->second];
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

} // namespace Elven::ecs
