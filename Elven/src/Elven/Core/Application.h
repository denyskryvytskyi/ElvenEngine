#pragma once

#include "Core.h"
#include "Elven/Events/Event.h"
#include "Elven/Events/ApplicationEvent.h"
#include "Window.h"

namespace Elven
{
    class ELVEN_API Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

        void OnEvent(Event& e);

    private:
        bool OnWindowClose(WindowCloseEvent& e);

    private:
        Scope<Window> m_Window;
        bool m_Running = true;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}


