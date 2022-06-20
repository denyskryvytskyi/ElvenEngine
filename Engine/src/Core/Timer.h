#pragma once

#include <chrono>

namespace Elven {

class Timer {
public:
    Timer()
        : begin(clock::now())
    {
    }

    // restart timer to current time
    void restart() { begin = clock::now(); }

    // get delta time as seconds
    float elapsed() const { return std::chrono::duration_cast<seconds>(clock::now() - begin).count(); }

private:
    using clock = std::chrono::high_resolution_clock; // QueryPerformanceCounter
    using seconds = std::chrono::duration<float, std::ratio<1>>;
    std::chrono::time_point<clock> begin;
};

} // namespace Elven
