#pragma once

#include <random>

namespace elv::ecs {

using Entity = uint64_t;
constexpr Entity INVALID_ENTITY_ID = 0;

static std::random_device s_randomDevice;
static std::mt19937_64 s_engine(s_randomDevice());
static std::uniform_int_distribution<Entity> s_uniformDistribution;

inline Entity GenerateEntityId()
{
    return s_uniformDistribution(s_engine);
}

} // namespace elv::ecs
