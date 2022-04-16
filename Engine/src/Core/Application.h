#pragma once

#include "Core/LayerStack.h"
#include "Events/ApplicationEvent.h"

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
        bool OnWindowResize(Events::WindowResizeEvent& e);

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
