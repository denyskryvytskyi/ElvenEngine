#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

namespace elv {
OpenGLContext::OpenGLContext()
    : m_windowHandle(nullptr)
{
}

void OpenGLContext::PreInit()
{
    // Note: at PreInit phase glfw window is NOT created yet!
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
}

void OpenGLContext::Init(void* wnd)
{
    m_windowHandle = static_cast<GLFWwindow*>(wnd);
    EL_CORE_ASSERT(m_windowHandle, "OpenGLContext window handle is null. Did you forget to create a window?");
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

    EL_CORE_ASSERT(GLAD_VERSION_MAJOR(status) > 4 || (GLAD_VERSION_MAJOR(status) == 4 && GLAD_VERSION_MINOR(status) >= 5), "Elven requires at least OpenGL version 4.5!");
}

void OpenGLContext::SwapBuffers()
{
    glfwSwapBuffers(m_windowHandle);
}
} // namespace elv
