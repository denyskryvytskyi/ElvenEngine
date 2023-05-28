#pragma once
#include <chrono>

namespace elv::time {
// Return seconds from epoch
float GetTimeSinceEpoch()
{
    return std::chrono::duration_cast<std::chrono::duration<float, std::ratio<1>>>(std::chrono::system_clock::now().time_since_epoch()).count();
}
} // namespace elv::time
