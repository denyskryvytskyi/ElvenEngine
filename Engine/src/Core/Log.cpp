#include "Core/Log.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace elv {

SharedPtr<spdlog::logger> Log::s_coreLogger;
SharedPtr<spdlog::logger> Log::s_clientLogger;

void Log::Init()
{
    std::vector<spdlog::sink_ptr> logSinks;
    logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    logSinks[0]->set_level(spdlog::level::trace);
    logSinks[0]->set_pattern("%^[%T] %n: %v%$");

    logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("logs/Elven.log", true));
    logSinks[1]->set_level(spdlog::level::trace);
    logSinks[1]->set_pattern("%^[%T] %n: %v%$");

    s_coreLogger = MakeSharedPtr<spdlog::logger>("ENGINE", logSinks.begin(), logSinks.end());
    s_coreLogger->set_level(spdlog::level::trace);

    s_clientLogger = MakeSharedPtr<spdlog::logger>("APP", logSinks.begin(), logSinks.end());
    s_clientLogger->set_level(spdlog::level::trace);
}

} // namespace elv
