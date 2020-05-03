#include "elpch.h"
#include "Application.h"

#include "Elven/Events/ApplicationEvent.h"
#include "Elven/Core/Log.h"

namespace Elven
{
    Application::Application()
    {
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        WindowResizeEvent e(1280, 720);
        if (e.IsInCategory(EventCategory_Application))
        {
            EL_TRACE(e);
        }
        if (e.IsInCategory(EventCategory_Keybord))
        {
            EL_TRACE(e);
        }

        while (true);
    }
}