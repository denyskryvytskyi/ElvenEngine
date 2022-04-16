#include "Core/Application.h"
#include "Core/Input.h"
#include "Core/Window.h"
#include "Core/Timer.h"

#include "Events/EventDispatcher.h"

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

        Events::Subscribe<Events::WindowCloseEvent>(EVENT_CALLBACK(Application::OnEvent));
        Events::Subscribe<Events::WindowResizeEvent>(EVENT_CALLBACK(Application::OnEvent));
    }

    Application::~Application()
    {
        delete m_Window;

        Events::Unsubscribe<Events::WindowResizeEvent>(EVENT_CALLBACK(Application::OnEvent));
        Events::Unsubscribe<Events::WindowCloseEvent>(EVENT_CALLBACK(Application::OnEvent));
        Events::gEventManager.Shutdown();
    }

    void Application::OnEvent(Events::Event& e)
    {
        Events::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<Events::WindowCloseEvent>(EVENT_CALLBACK(Application::OnWindowClose));
        dispatcher.Dispatch<Events::WindowResizeEvent>(EVENT_CALLBACK(Application::OnWindowResize));
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

    bool Application::OnWindowClose(Events::WindowCloseEvent& e)
    {
        m_Running = false;

        return true;
    }

    bool Application::OnWindowResize(Events::WindowResizeEvent& e)
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