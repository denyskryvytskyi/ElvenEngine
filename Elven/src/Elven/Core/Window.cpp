#include "elpch.h"
#include "Elven/Core/Window.h"

#ifdef EL_PLATFORM_WINDOWS
    #include "Platform/Windows/WindowsWindow.h"
#endif

namespace Elven
{
    Window* Elven::Window::Create(const WindowProps& props)
    {
        #ifdef EL_PLATFORM_WINDOWS
            return new WindowsWindow(props);
        #else
            EL_CORE_ASSERT(false, "Unknown platform!");
            retur nullptr;
        #endif
    }
}
