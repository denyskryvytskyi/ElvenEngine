#include "Core/Window.h"

#include "Platform/Windows/WindowsWindow.h"

namespace elv {

UniquePtr<Window> Window::Create(const WindowProps& props)
{
    // #ifdef EL_PLATFORM_WINDOWS
    //     rand_r
    // #else
    //     EL_CORE_ASSERT(false, "Unknown platform!");
    //     return nullptr;
    // #endif
    return MakeUniquePtr<WindowsWindow>(props);
}

} // namespace elv
