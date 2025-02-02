#include "Core/Window.h"

#ifdef EL_PLATFORM_WINDOWS
#    include "Platform/Windows/WindowsWindow.h"
#endif

namespace elv {

UniquePtr<Window> Window::Create(const WindowProps& props)
{
#ifdef EL_PLATFORM_WINDOWS
    return MakeUniquePtr<WindowsWindow>(props);
#else
    EL_CORE_ASSERT(false, "Unknown platform!");
    return nullptr;
#endif
}

} // namespace elv
