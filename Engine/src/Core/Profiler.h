#pragma once

#include "Core/Timer.h"
#include "Log.h"

#include <string>

namespace elv {
class ProfileTelemetry {
public:
    static ProfileTelemetry& GetInstance()
    {
        static ProfileTelemetry instance;
        return instance;
    }

    const std::unordered_map<std::string, float>& GetTelemetry() const { return m_telemetry; }

    void Update(const std::string& name, const float value);

private:
    std::unordered_map<std::string, float> m_telemetry;
};

extern ProfileTelemetry gProfileTelemetry;

class Profiler {
public:
    Profiler(const std::string& text, const bool writeToLog = false)
        : m_text(text)
        , m_writeToLog(writeToLog)
    { }

    ~Profiler()
    {
        const float elapsed = m_timer.ElapsedMs();
        if (m_writeToLog) {
            EL_INFO("{0} {1} ms.", m_text, elapsed);

        } else {
            ProfileTelemetry::GetInstance().Update(m_text, elapsed);
        }
    }

private:
    bool m_writeToLog { false };
    std::string m_text;
    Timer m_timer;
};

#ifdef PROFILE_MODE
#    define PROFILE(name) Profiler p(name);

#    define PROFILE_SCOPE(name) Profiler p(name);

#    define PROFILE_TO_LOG(name) Profiler p(name, true);

#else
#    define PROFILE(...)
#    define PROFILE_SCOPE(...)
#endif
} // namespace elv
