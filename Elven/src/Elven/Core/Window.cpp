#include "elpch.h"
#include "Window.h"

#ifdef EL_PLATFORM_WINDOWS
    #include "Platform/Windows/WindowsWindow.h"
#endif

namespace Elven
{
    Scope<Window> Elven::Window::Create(const WindowProps& props)
    {
        #ifdef EL_PLATFORM_WINDOWS
            return CreateScope<WindowsWindow>(props);
        #else
            EL_CORE_ASSERT(false, "Unknown platform!");
            retur nullptr;
        #endif
    }
}
