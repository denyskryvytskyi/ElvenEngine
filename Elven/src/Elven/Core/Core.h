#pragma once

#ifdef EL_PLATFORM_WINDOWS
    #ifdef EL_BUILD_DLL
        #define ELVEN_API __declspec(dllexport)
    #else
        #define ELVEN_API __declspec(dllimport)
    #endif
#else
    #error Elven only support Windows!
#endif

#define BIT(x) (1 << x)