#pragma once

#include "Core.h"
#include "Window.h"

namespace Elven
{
    class ELVEN_API Application
    {
    public:
        Application();
        virtual ~Application();
        //
        void Run();
    private:
        Scope<Window> m_Window;
        bool m_Running = true;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}


