#pragma once

#include <string>

namespace Elven {

inline uint64_t string_id(const std::string& str)
{
    return std::hash<std::string>()(str);
}
} // namespace Elven
