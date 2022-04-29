#include "Core/Application.h"
#include "Core/Input.h"
#include "Core/Window.h"
#include "Core/Timer.h"

#include "Renderer/Renderer.h"

#include "ImGui/ImGuiLayer.h"

namespace Elven
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
        : m_Running(true)
    {
        EL_CORE_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = Window::Create();
        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        m_windowCloseCallback = EVENT_CALLBACK(Application::OnWindowClose);
        m_windowResizeCallback = EVENT_CALLBACK(Application::OnWindowResize);

        Events::Subscribe<Events::WindowCloseEvent>(m_windowCloseCallback);
        Events::Subscribe<Events::WindowResizeEvent>(m_windowResizeCallback);
    }

    Application::~Application()
    {
        delete m_Window;

        Events::Unsubscribe<Events::WindowResizeEvent>(m_windowResizeCallback);
        Events::Unsubscribe<Events::WindowCloseEvent>(m_windowCloseCallback);

        Events::gEventManager.Shutdown();
    }

    void Application::PushLayer(Layer* layer)
    {
        m_LayerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_LayerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Application::Run()
    {
        Timer timer;
        while (m_Running)
        {
            float elapsedTime = timer.elapsed();
            timer.restart();

            // Layers update
            for (Layer* layer : m_LayerStack)
            {
                layer->OnUpdate(elapsedTime);
            }

            // ImGui layers render
            m_ImGuiLayer->Begin();
            for (Layer* layer : m_LayerStack)
            {
                layer->OnImGuiRender();
            }
            m_ImGuiLayer->End();

            // Window update
            m_Window->OnUpdate();

            // Dispatch queued events
            Events::gEventManager.DispatchEvents();
        }
    }

    bool Application::OnWindowClose(const Events::WindowCloseEvent& e)
    {
        m_Running = false;

        return true;
    }

    bool Application::OnWindowResize(const Events::WindowResizeEvent& e)
    {
        if (e.GetWidth() == 0 || e.GetHeight() == 0)
        {
            // minimized logic
            return true;
        }

        Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());

        return true;
    }
}