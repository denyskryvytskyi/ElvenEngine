#pragma once

#include <memory>

#ifdef DEBUG_MODE
#    if defined(EL_PLATFORM_WINDOWS)
#        define EL_DEBUGBREAK() __debugbreak()

// To detect memory leaks using CRT lib
#        define _CRTDBG_MAP_ALLOC
#        include <crtdbg.h>
#        define DBG_NEW new (_NORMAL_BLOCK, __FILE__, __LINE__)
#    elif defined(EL_PLATFORM_LINUX)
#        include <signal.h>
#        define EL_DEBUGBREAK() raise(SIGTRAP)
#    else
#        error "Platform doesn't support debugbreak yet!"
#    endif
#    define EL_ASSERTIONS_ENABLED
#else
#    define EL_DEBUGBREAK()
#    define DBG_NEW
#endif

#ifdef EL_ASSERTIONS_ENABLED

#    define EL_ASSERT(expr, ...)                               \
        if (expr) {                                            \
        } else {                                               \
            EL_CRITICAL("Assertion Failed: {0}", __VA_ARGS__); \
            EL_DEBUGBREAK();                                   \
        }

#    define EL_CORE_ASSERT(expr, ...)                               \
        if (expr) {                                                 \
        } else {                                                    \
            EL_CORE_CRITICAL("Assertion Failed: {0}", __VA_ARGS__); \
            EL_DEBUGBREAK();                                        \
        }
#else
#    define EL_ASSERT(expr, ...)
#    define EL_CORE_ASSERT(expr, ...)
#endif

#define BIT(x) (1 << x)

namespace elv {
template<typename T>
using UniquePtr = std::unique_ptr<T>;
template<typename T, typename... Args>
constexpr UniquePtr<T> MakeUniquePtr(Args&&... args)
{
    return std::make_unique<T>(std::forward<Args>(args)...);
}

template<typename T>
using SharedPtr = std::shared_ptr<T>;
template<typename T, typename... Args>
constexpr SharedPtr<T> MakeSharedPtr(Args&&... args)
{
    return std::make_shared<T>(std::forward<Args>(args)...);
}

} // namespace elv
