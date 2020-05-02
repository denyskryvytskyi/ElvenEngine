#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Elven
{
    class ELVEN_API Log
    {
    public:
        static void Init();

        static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }
        static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }

    private:
        static std::shared_ptr<spdlog::logger> s_CoreLogger;
        static std::shared_ptr<spdlog::logger> s_ClientLogger;
    };

}

// Core log macros
#define EL_CORE_TRACE(...)  ::Elven::Log::GetCoreLogger()->trace(__VA_ARGS__)
#define EL_CORE_INFO(...)   ::Elven::Log::GetCoreLogger()->info(__VA_ARGS__)
#define EL_CORE_WARN(...)   ::Elven::Log::GetCoreLogger()->warn(__VA_ARGS__)
#define EL_CORE_ERROR(...)  ::Elven::Log::GetCoreLogger()->error(__VA_ARGS__)
#define EL_CORE_CRITICAL(...)  ::Elven::Log::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define EL_TRACE(...)       ::Elven::Log::GetClientLogger()->trace(__VA_ARGS__)
#define EL_INFO(...)        ::Elven::Log::GetClientLogger()->info(__VA_ARGS__)
#define EL_WARN(...)        ::Elven::Log::GetClientLogger()->warn(__VA_ARGS__)
#define EL_ERROR(...)       ::Elven::Log::GetClientLogger()->error(__VA_ARGS__)
#define EL_CRITICAL(...)    ::Elven::Log::GetClientLogger()->critical(__VA_ARGS__)