#pragma once

#include "Elven/Core/Window.h"
#include "Elven/Core/LayerStack.h"
#include "Elven/Core/Timer.h"

#include "Elven/ImGui/ImGuiLayer.h"

namespace Elven
{
    class Application
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

        Scope<Window> m_Window;
        ImGuiLayer* m_ImGuiLayer;
        LayerStack m_LayerStack;

        bool m_Running;
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}
