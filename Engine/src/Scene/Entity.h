#pragma once

#include <random>

namespace elv::ecs {

using Entity = uint64_t;
constexpr Entity INVALID_ENTITY_ID = 0;

inline Entity GenerateEntityId()
{
    static std::atomic<Entity> next { INVALID_ENTITY_ID + 1 };
    return next.fetch_add(1);
}

} // namespace elv::ecs
