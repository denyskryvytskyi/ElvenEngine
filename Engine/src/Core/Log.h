#pragma once

#pragma warning(push, 0)
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#pragma warning(pop)

namespace Elven {

class Log {
public:
    static void Init();

    const static SharedPtr<spdlog::logger>& GetCoreLogger() { return s_coreLogger; }
    const static SharedPtr<spdlog::logger>& GetClientLogger() { return s_clientLogger; }

private:
    static SharedPtr<spdlog::logger> s_coreLogger;
    static SharedPtr<spdlog::logger> s_clientLogger;
};

} // namespace Elven

// Core log macros
template<typename... Args>
static void EL_CORE_INFO(Args&&... args)
{
    Elven::Log::GetCoreLogger()->info(std::forward<Args>(args)...);
}

template<typename... Args>
static void EL_CORE_WARN(Args&&... args)
{
    Elven::Log::GetCoreLogger()->warn(std::forward<Args>(args)...);
}

template<typename... Args>
static void EL_CORE_ERROR(Args&&... args)
{
    Elven::Log::GetCoreLogger()->error(std::forward<Args>(args)...);
}

template<typename... Args>
static void EL_CORE_CRITICAL(Args&&... args)
{
    Elven::Log::GetCoreLogger()->critical(std::forward<Args>(args)...);
}

#define EL_CORE_TRACE(...) ::Elven::Log::GetCoreLogger()->trace(__VA_ARGS__)
//#define EL_CORE_INFO(...) ::Elven::Log::GetCoreLogger()->info(__VA_ARGS__)
//#define EL_CORE_WARN(...) ::Elven::Log::GetCoreLogger()->warn(__VA_ARGS__)
//#define EL_CORE_ERROR(...) ::Elven::Log::GetCoreLogger()->error(__VA_ARGS__)
//#define EL_CORE_CRITICAL(...) ::Elven::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define EL_TRACE(...) ::Elven::Log::GetClientLogger()->trace(__VA_ARGS__)
#define EL_INFO(...) ::Elven::Log::GetClientLogger()->info(__VA_ARGS__)
#define EL_WARN(...) ::Elven::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EL_ERROR(...) ::Elven::Log::GetClientLogger()->error(__VA_ARGS__)
#define EL_CRITICAL(...) ::Elven::Log::GetClientLogger()->critical(__VA_ARGS__)
