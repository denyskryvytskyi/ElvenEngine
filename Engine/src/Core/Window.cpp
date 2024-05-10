#include "Core/Window.h"

#include "Platform/Windows/WindowsWindow.h"

namespace elv {

UniquePtr<Window> Window::Create(const WindowProps& props)
{
    return MakeUniquePtr<WindowsWindow>(props);
}

} // namespace elv
