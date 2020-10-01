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
    }

    void OpenGLContext::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }
}
