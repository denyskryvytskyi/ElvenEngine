#pragma once

#define SPDLOG_FMT_EXTERNAL
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>

namespace elv {

class Log {
public:
    static void Init();

    const static SharedPtr<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }
    const static SharedPtr<spdlog::logger>& GetClientLogger() { return s_clientLogger; }

private:
    static SharedPtr<spdlog::logger> s_coreLogger;
    static SharedPtr<spdlog::logger> s_clientLogger;
};

} // namespace elv

// Engine logging
#define EL_CORE_TRACE(...) elv::Log::GetCoreLogger()->trace(__VA_ARGS__);
#define EL_CORE_INFO(...) elv::Log::GetCoreLogger()->info(__VA_ARGS__);
#define EL_CORE_WARN(...) elv::Log::GetCoreLogger()->warn(__VA_ARGS__);
#define EL_CORE_ERROR(...) elv::Log::GetCoreLogger()->error(__VA_ARGS__);
#define EL_CORE_CRITICAL(...) elv::Log::GetCoreLogger()->critical(__VA_ARGS__);

// Client logging
#define EL_TRACE(...) elv::Log::GetClientLogger()->trace(__VA_ARGS__);
#define EL_INFO(...) elv::Log::GetClientLogger()->info(__VA_ARGS__);
#define EL_WARN(...) elv::Log::GetClientLogger()->warn(__VA_ARGS__);
#define EL_ERROR(...) elv::Log::GetClientLogger()->error(__VA_ARGS__);
#define EL_CRITICAL(...) elv::Log::GetClientLogger()->critical(__VA_ARGS__);
