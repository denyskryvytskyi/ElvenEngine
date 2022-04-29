#pragma once

#include "Core/LayerStack.h"
#include "Events/ApplicationEvent.h"

#include "Events/EventManager.h"

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

        void PushLayer(Layer* layer);
        void PushOverlay(Layer* overlay);

        void Run();

        static Application& Get() { return *s_Instance; }
        Window& GetWindow() { return *m_Window; }

    private:
        bool OnWindowClose(const Events::WindowCloseEvent& e);
        bool OnWindowResize(const Events::WindowResizeEvent& e);

    private:
        static Application* s_Instance;

        Window* m_Window = nullptr;
        ImGuiLayer* m_ImGuiLayer = nullptr;
        LayerStack m_LayerStack;

        Events::EventFunctionHandler<Events::WindowResizeEvent> m_windowResizeCallback;
        Events::EventFunctionHandler<Events::WindowCloseEvent> m_windowCloseCallback;

        bool m_Running = false;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}
