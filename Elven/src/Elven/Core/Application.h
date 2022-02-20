#pragma once

#include "Elven/Core/LayerStack.h"

#include "Elven/Events/EventDispatcher.h"
#include "Elven/Events/ApplicationEvent.h"

namespace Elven
{
    class Window;
    class Layer;
    class ImGuiLayer;

    class Application
    {
    public:
        Application();
        virtual ~Application();

        void OnEvent(Events::Event& e);

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        void Run();

        static Application& Get() { return *s_Instance; }
        Window& GetWindow() { return *m_Window; }

    private:
        bool OnWindowClose(Events::WindowCloseEvent& e);

    private:
        static Application* s_Instance;

        Window* m_Window = nullptr;
        ImGuiLayer* m_ImGuiLayer = nullptr;
        LayerStack m_LayerStack;

        bool m_Running = false;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}
