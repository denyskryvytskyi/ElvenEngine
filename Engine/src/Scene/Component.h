#pragma once

#include "Entity.h"

namespace Elven::ecs {

using ComponentTypeId = std::uint64_t;

constexpr ComponentTypeId INVALID_COMPONENT_TYPE_ID = 0;
constexpr std::uint32_t MAX_ENTITIES_PER_COMPONENT = 2'000'0000;

namespace {
ComponentTypeId componentTypeCounter = 0;
}

template<class ComponentType>
ComponentTypeId GetComponentTypeId()
{
    static ComponentTypeId id = INVALID_COMPONENT_TYPE_ID;
    if (id == INVALID_COMPONENT_TYPE_ID) {
        id = static_cast<ComponentTypeId>(1) << componentTypeCounter++;
    }

    return id;
}

struct ComponentArrayBase {
    virtual ~ComponentArrayBase() = default;

    virtual void Clear() = 0;
    virtual void AddComponent(EntityId entityId) = 0;
    virtual void RemoveComponent(EntityId entityId) = 0;
    virtual EntityId GetEntity(std::uint32_t componentIndex) const = 0;
};

template<class ComponentType>
class ComponentArray final : public ComponentArrayBase {
public:
    ComponentArray()
    {
        m_components.reserve(MAX_ENTITIES_PER_COMPONENT);
    }

    void Clear() override
    {
        m_components.clear();
        m_entityToComponentIndex.clear();
        m_componentToEntities.clear();
    }

    void AddComponent(EntityId entityId) override
    {
        if (m_components.size() < MAX_ENTITIES_PER_COMPONENT) {
            m_entityToComponentIndex.insert({ entityId, m_components.size() });
            m_components.emplace_back();
            m_componentToEntities.emplace_back(entityId);
        } else {
            EL_CORE_CRITICAL("Max components limit reached.");
        }
    }

    void RemoveComponent(EntityId entityId) override
    {
        auto it = m_entityToComponentIndex.find(entityId);
        if (it != m_entityToComponentIndex.end()) {
            const std::uint64_t componentIndex = it->second;

            if (componentIndex < m_components.size() - 1) {
                m_components[componentIndex] = std::move(m_components.back()); // replace dead component with the last one

                EntityId movedComponentEntityId = m_componentToEntities.back();
                m_entityToComponentIndex[movedComponentEntityId] = componentIndex; // new mapping for moved component
            }
        }

        m_components.pop_back();
        m_componentToEntities.pop_back();
        m_entityToComponentIndex.erase(it);
    }

    EntityId GetEntity(std::uint32_t componentIndex) const override
    {
        return m_componentToEntities[componentIndex];
    }

    ComponentType& GetComponent(EntityId entityId)
    {
        auto it = m_entityToComponentIndex.find(entityId);

        if (it == m_entityToComponentIndex.end()) {
            EL_CORE_CRITICAL("There isn't component with entity id = {0}", entityId);
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
    std::unordered_map<EntityId, std::uint64_t> m_entityToComponentIndex;

    // index - component index that is the same for index from m_components
    // value - entity id associated with this component
    std::vector<EntityId> m_componentToEntities;
};

} // namespace Elven::ecs
