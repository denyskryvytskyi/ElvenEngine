#pragma once

#include <memory>

#ifdef EL_DEBUG
    #if defined(EL_PLATFORM_WINDOWS)
        #define EL_DEBUGBREAK() __debugbreak()
    #elif defined(EL_PLATFORM_LINUX)
        #include <signal.h>
        #define EL_DEBUGBREAK() raise(SIGTRAP)
    #else
        #error "Platform doesn't support debugbreak yet!"
    #endif
    #define EL_ASSERTIONS_ENABLED
#else
    #define EL_DEBUGBREAK()
#endif

#ifdef EL_ASSERTIONS_ENABLED

    #define EL_ASSERT(expr, ...) if (expr) {} \
        else \
        { \
            EL_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
            EL_DEBUGBREAK(); \
        }

    #define EL_CORE_ASSERT(expr, ...) if (expr) {} \
        else \
        { \
            EL_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); \
            EL_DEBUGBREAK(); \
        }
#else
    #define EL_ASSERT(expr)
    #define EL_CORE_ASSERT(expr)
#endif

#define BIT(x) (1 << x)

#define EL_BIND_EVENT_FN(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Elven
{
    template<typename T>
    using Scope = std::unique_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Scope<T> CreateScope(Args&& ... args)
    {
        return std::make_unique<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;
    template<typename T, typename ... Args>
    constexpr Ref<T> CreateRef(Args&& ...args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    static void DeleteRawPointer(void* ptr)
    {
        if (ptr)
        {
            delete ptr;
        }
    }
}