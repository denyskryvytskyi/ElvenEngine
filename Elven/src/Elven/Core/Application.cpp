#include "elpch.h"

#include "Elven/Core/Application.h"
#include "Elven/Core/Input.h"

#include "Elven/Renderer/Renderer.h"
#include "Elven/Events/EventManager.h"
#include "Elven/Events/EventDispatcher.h"

namespace Elven
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
        : m_Running(true)
    {
        EL_CORE_ASSERT(!s_Instance, "Application already exist!");
        s_Instance = this;

        m_Window = Window::Create();
        gEventManager.Subscribe(WindowCloseEvent::GetStaticUUID(), EL_BIND_EVENT_FN(Application::OnEvent));
        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);
    }

    Application::~Application()
    {
        DeleteRawPointer(m_Window);

        gEventManager.Shutdown();
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(EL_BIND_EVENT_FN(Application::OnWindowClose));
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
            gEventManager.DispatchEvents();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;

        return true;
    }
}