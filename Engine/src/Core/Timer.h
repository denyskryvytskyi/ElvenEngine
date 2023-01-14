#pragma once

#include <chrono>

namespace elv {

class Timer {
public:
    Timer()
        : m_start(clock::now())
    {
    }

    // restart timer to current time
    void Restart() { m_start = clock::now(); }

    // get delta time as seconds
    float Elapsed() const { return std::chrono::duration_cast<seconds>(clock::now() - m_start).count(); }

private:
    using clock = std::chrono::high_resolution_clock; // QueryPerformanceCounter
    using seconds = std::chrono::duration<float, std::ratio<1>>;

    std::chrono::time_point<clock> m_start;
};

} // namespace elv
