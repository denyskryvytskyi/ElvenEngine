#include "Core/Application.h"
#include "Core/Input.h"
#include "Core/Window.h"
#include "Core/Timer.h"

#include "Renderer/Renderer.h"

#include "ImGui/ImGuiLayer.h"

namespace Elven
{
    Application* Application::s_instance = nullptr;
    Application::Telemetry Application::s_telemetry;

    Application::Application()
        : m_running(true)
    {
        EL_CORE_ASSERT(!s_instance, "Application already exists!");
        s_instance = this;

        m_window = Window::Create();
        m_imGuiLayer = new ImGuiLayer();
        PushOverlay(m_imGuiLayer);

        Elven::Renderer::Init();

        m_windowCloseCallback = EVENT_CALLBACK(Application::OnWindowClose);
        m_windowResizeCallback = EVENT_CALLBACK(Application::OnWindowResize);

        Events::Subscribe<Events::WindowCloseEvent>(m_windowCloseCallback);
        Events::Subscribe<Events::WindowResizeEvent>(m_windowResizeCallback);
    }

    Application::~Application()
    {
        delete m_window;

        Events::Unsubscribe<Events::WindowResizeEvent>(m_windowResizeCallback);
        Events::Unsubscribe<Events::WindowCloseEvent>(m_windowCloseCallback);

        Events::gEventManager.Shutdown();
    }

    void Application::PushLayer(Layer* layer)
    {
        m_layerStack.PushLayer(layer);
        layer->OnAttach();
    }

    void Application::PushOverlay(Layer* overlay)
    {
        m_layerStack.PushOverlay(overlay);
        overlay->OnAttach();
    }

    void Application::Run()
    {
        Timer timer;
        while (m_running)
        {
            float elapsedTime = timer.elapsed();
            timer.restart();

            s_telemetry.frameTime = elapsedTime * 1000.0f;
            s_telemetry.fps = 1.0f / elapsedTime;

            // Layers update
            for (Layer* layer : m_layerStack)
            {
                layer->OnUpdate(elapsedTime);
            }

            // ImGui layers render
            m_imGuiLayer->Begin();
            for (Layer* layer : m_layerStack)
            {
                layer->OnImGuiRender();
            }
            m_imGuiLayer->End();

            // Window update
            m_window->OnUpdate();

            // Dispatch queued events
            Events::gEventManager.DispatchEvents();
        }
    }

    bool Application::OnWindowClose(const Events::WindowCloseEvent& e)
    {
        m_running = false;

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