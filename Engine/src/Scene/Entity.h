#pragma once

#include <bitset>
#include <random>

namespace Elven::ecs {

using EntityId = std::uint64_t;
constexpr EntityId INVALID_ENTITY_ID = 0;

constexpr std::uint32_t MAX_COMPONENTS = 32;
using ComponentMask = std::bitset<MAX_COMPONENTS>;

static std::random_device s_randomDevice;
static std::mt19937_64 s_engine(s_randomDevice());
static std::uniform_int_distribution<EntityId> s_uniformDistribution;

static EntityId CreateEntityId()
{
    return s_uniformDistribution(s_engine);
}

struct Entity {
    Entity(EntityId id_, ComponentMask componentMask_)
        : id(id_)
        , componentMask(componentMask_)
    { }

    EntityId id;
    ComponentMask componentMask;
};

} // namespace Elven::ecs
