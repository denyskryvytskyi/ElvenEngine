#pragma once

#include <chrono>

namespace Elven
{
    class Timer
    {
    public:
        Timer()
            : begin(clock::now())
        {}

        // restart timer to current time
        void restart() { begin = clock::now(); }

        // get delta time as seconds
        float elapsed() const { return std::chrono::duration_cast<second>(clock::now() - begin).count(); }

    private:
        typedef std::chrono::high_resolution_clock clock; // QueryPerformanceCounter
        typedef std::chrono::duration<float, std::ratio<1>> second;
        std::chrono::time_point<clock> begin;
    };
}