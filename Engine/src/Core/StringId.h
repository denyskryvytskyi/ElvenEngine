#pragma once

#include <string>

namespace elv {

inline uint64_t string_id(std::string_view str)
{
    return std::hash<std::string_view>()(str);
}
} // namespace elv
