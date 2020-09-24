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

        static Application& Get() { return *s_Instance; }
        Window& GetWindow() { return *m_Window; }

    private:
        bool OnWindowClose(WindowCloseEvent& e);

    private:
        static Application* s_Instance;

        LayerStack m_LayerStack;
        Scope<Window> m_Window;
        bool m_Running = true;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}


