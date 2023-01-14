#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/gl.h>

namespace elv {
OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
    : m_windowHandle(windowHandle)
{
}

void OpenGLContext::Init()
{
    glfwMakeContextCurrent(m_windowHandle);

    int status = gladLoadGL(glfwGetProcAddress);
    EL_CORE_ASSERT(status, "Failed to initialize Glad.");

    EL_CORE_INFO("OpenGL Info:");
    EL_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
    EL_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
    EL_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

    EL_CORE_ASSERT(GLAD_VERSION_MAJOR(status) > 4 || (GLAD_VERSION_MAJOR(status) == 4 && GLAD_VERSION_MINOR(status) >= 5), "Elven requires at least OpenGL version 4.5!");
}

void OpenGLContext::SwapBuffers()
{
    glfwSwapBuffers(m_windowHandle);
}
} // namespace elv
