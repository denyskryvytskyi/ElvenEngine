#pragma once

#include "Elven/Core/Core.h"
#include "Elven/Events/Event.h"
#include "Elven/Events/ApplicationEvent.h"
#include "Elven/Core/Window.h"
#include "Elven/Core/LayerStack.h"

namespace Elven
{
    class ELVEN_API Application
    {
    public:
        Application();
        virtual ~Application();

        void OnEvent(Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        void Run();

    private:
        bool OnWindowClose(WindowCloseEvent& e);

    private:
        LayerStack m_LayerStack;
        Scope<Window> m_Window;
        bool m_Running = true;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}


