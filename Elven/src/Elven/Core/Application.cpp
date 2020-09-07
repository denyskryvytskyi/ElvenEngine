#include "elpch.h"
#include "Application.h"

#include "Elven/Events/ApplicationEvent.h"
#include "Elven/Core/Log.h"

#include <GLFW/glfw3.h>

namespace Elven
{
    Application::Application()
    {
        m_Window = Window::Create();
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(0, 1, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT);
            m_Window->OnUpdate();
        }
    }
}