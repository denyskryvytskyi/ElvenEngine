#include "Platform/OpenGL41/OpenGL41Context.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

namespace elv {
OpenGL41Context::OpenGL41Context(GLFWwindow* windowHandle)
    : m_windowHandle(windowHandle)
{
}

void OpenGL41Context::Init()
{
    glfwMakeContextCurrent(m_windowHandle);

    int status = gladLoadGL(glfwGetProcAddress);
    EL_CORE_ASSERT(status, "Failed to initialize Glad.");

    EL_CORE_INFO("OpenGL Info:");
    EL_CORE_INFO("  Vendor: {0}", (const char*)glGetString(GL_VENDOR));
    EL_CORE_INFO("  Renderer: {0}", (const char*)glGetString(GL_RENDERER));
    EL_CORE_INFO("  Version: {0}", (const char*)glGetString(GL_VERSION));

    int maxSlots;
    glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &maxSlots);
    EL_CORE_INFO("  Max texture slots: {0}", maxSlots);

    EL_CORE_ASSERT(GLAD_VERSION_MAJOR(status) > 4 || (GLAD_VERSION_MAJOR(status) == 4 && GLAD_VERSION_MINOR(status) >= 1), "Elven requires at least OpenGL version 4.1!");
}

void OpenGL41Context::SwapBuffers()
{
    glfwSwapBuffers(m_windowHandle);
}
} // namespace elv
