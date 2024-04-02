#pragma once

#include "Profiler.h"

namespace elv {
void ProfileTelemetry::Update(const std::string& name, const float value)
{
    m_telemetry[name] = value;
}
} // namespace elv
