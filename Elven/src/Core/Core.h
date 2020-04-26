#pragma once

#ifdef EV_PLATFORM_WINDOWS
    #ifdef EV_BUILD_DLL
        #define ELVEN_API __declspec(dllexport)
    #else
        #define ELVEN_API __declspec(dllimport)
    #endif
#else
    #error Elven only support Windows!
#endif