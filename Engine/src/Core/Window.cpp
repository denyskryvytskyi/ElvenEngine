#include "Core/Window.h"

#include "Platform/GLFW/GLFWWindowImpl.h"

namespace elv {

UniquePtr<Window> Window::Create(const WindowProps& props)
{
    return MakeUniquePtr<GLFWWindowImpl>(props);
}

} // namespace elv
