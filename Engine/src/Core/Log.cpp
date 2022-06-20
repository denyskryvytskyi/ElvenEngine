#include "Core/Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>

namespace Elven {

SharedPtr<spdlog::logger> Log::s_coreLogger;
SharedPtr<spdlog::logger> Log::s_clientLogger;

void Elven::Log::Init()
{
    spdlog::set_pattern("%^[%T] %n: %v%$");
    s_coreLogger = spdlog::stdout_color_mt("ELVEN");
    s_coreLogger->set_level(spdlog::level::trace);

    s_clientLogger = spdlog::stdout_color_mt("APP");
    s_clientLogger->set_level(spdlog::level::trace);
}

} // namespace Elven
