#include "elpch.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Elven
{
    OpenGLContext::OpenGLContext(GLFWwindow* windowHandle)
        : m_WindowHandle(windowHandle)
    {
    }

    void OpenGLContext::Init()
    {
        glfwMakeContextCurrent(m_WindowHandle);

        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        EL_CORE_ASSERT(status, "Failed to initialize Glad.");

        EL_CORE_INFO("OpenGL Info:");
        EL_CORE_INFO("  Vendor: {0}", glGetString(GL_VENDOR));
        EL_CORE_INFO("  Renderer: {0}", glGetString(GL_RENDERER));
        EL_CORE_INFO("  Version: {0}", glGetString(GL_VERSION));

        EL_CORE_ASSERT(GLVersion.major > 4 || (GLVersion.major == 4 && GLVersion.minor >= 5), "Hazel requires at least OpenGL version 4.5!");
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }
}
