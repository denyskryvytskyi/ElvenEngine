#pragma once

#pragma warning(push, 0)
#include <spdlog/fmt/ostr.h>
#include <spdlog/spdlog.h>
#pragma warning(pop)

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
// template<typename... Args>
// static void EL_CORE_TRACE(Args&&... args)
//{
//    elv::Log::GetCoreLogger()->trace(std::forward<Args>(args)...);
//}
//
// template<typename... Args>
// static void EL_CORE_INFO(Args&&... args)
//{
//    elv::Log::GetCoreLogger()->info(std::forward<Args>(args)...);
//}
//
// template<typename... Args>
// static void EL_CORE_WARN(Args&&... args)
//{
//    elv::Log::GetCoreLogger()->warn(std::forward<Args>(args)...);
//}
//
// template<typename... Args>
// static void EL_CORE_ERROR(Args&&... args)
//{
//    elv::Log::GetCoreLogger()->error(std::forward<Args>(args)...);
//}
//
// template<typename... Args>
// static void EL_CORE_CRITICAL(Args&&... args)
//{
//    elv::Log::GetCoreLogger()->critical(std::forward<Args>(args)...);
//}

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

// template<typename... Args>
// static void EL_TRACE(Args&&... args)
//{
//     elv::Log::GetClientLogger()->trace(std::forward<Args>(args)...);
// }
//
// template<typename... Args>
// static void EL_INFO(Args&&... args)
//{
//     elv::Log::GetClientLogger()->info(std::forward<Args>(args)...);
// }
//
// template<typename... Args>
// static void EL_WARN(Args&&... args)
//{
//     elv::Log::GetClientLogger()->warn(std::forward<Args>(args)...);
// }
//
// template<typename... Args>
// static void EL_ERROR(Args&&... args)
//{
//     elv::Log::GetClientLogger()->error(std::forward<Args>(args)...);
// }
//
// template<typename... Args>
// static void EL_CRITICAL(Args&&... args)
//{
//     elv::Log::GetClientLogger()->critical(std::forward<Args>(args)...);
// }
