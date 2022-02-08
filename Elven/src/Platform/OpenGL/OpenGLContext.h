#pragma once

#include "Elven/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Elven
{
    class OpenGLContext : public GraphicsContext
    {
    public:
        OpenGLContext(GLFWwindow* windowHandle);

        void Init() override;
        void SwapBuffers() override;
    private:
        GLFWwindow* m_WindowHandle = nullptr;
    };
}

