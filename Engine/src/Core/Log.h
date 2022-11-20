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

// Engine logging
template<typename... Args>
static void EL_CORE_TRACE(Args&&... args)
{
    Elven::Log::GetCoreLogger()->trace(std::forward<Args>(args)...);
}

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

// Client logging
template<typename... Args>
static void EL_TRACE(Args&&... args)
{
    Elven::Log::GetClientLogger()->trace(std::forward<Args>(args)...);
}

template<typename... Args>
static void EL_INFO(Args&&... args)
{
    Elven::Log::GetClientLogger()->info(std::forward<Args>(args)...);
}

template<typename... Args>
static void EL_WARN(Args&&... args)
{
    Elven::Log::GetClientLogger()->warn(std::forward<Args>(args)...);
}

template<typename... Args>
static void EL_ERROR(Args&&... args)
{
    Elven::Log::GetClientLogger()->error(std::forward<Args>(args)...);
}

template<typename... Args>
static void EL_CRITICAL(Args&&... args)
{
    Elven::Log::GetClientLogger()->critical(std::forward<Args>(args)...);
}
