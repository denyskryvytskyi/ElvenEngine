#pragma once

#include "Core/Timer.h"
#include "Log.h"

#include <string>

namespace elv {
class Profiler {
public:
    Profiler(const std::string& text)
        : m_text(text)
    { }

    ~Profiler()
    {
        EL_INFO("{0} {1} ms.", m_text, m_timer.ElapsedMs());
    }

private:
    std::string m_text;
    Timer m_timer;
};

#ifdef PROFILE_MODE
#    define PROFILE(name) Profiler p(name);

#    define PROFILE_SCOPE(name) Profiler p(name);

#else
#    define PROFILE(...)
#    define PROFILE_SCOPE(...)
#endif
} // namespace elv
