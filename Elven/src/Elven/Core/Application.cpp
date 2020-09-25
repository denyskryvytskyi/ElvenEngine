#include "elpch.h"
#include "Application.h"

#include <glad/glad.h>

namespace Elven
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        EL_CORE_ASSERT(!s_Instance, "Application already exist!");
        s_Instance = this;

        m_Window = Window::Create();
        m_Window->SetEventCallback(EL_BIND_EVENT_FN(Application::OnEvent));

        unsigned int id;
        glGenVertexArrays(1, &id);
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(EL_BIND_EVENT_FN(Application::OnWindowClose));

        EL_CORE_TRACE(e.ToString());

        for (auto it = m_LayerStack.rbegin(); it != m_LayerStack.rend(); ++it)
        {
            if (e.Handled)
                break;
            (*it)->OnEvent(e);
        }
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
        while (m_Running)
        {
            glClearColor(0, 1, 0, 0);
            glClear(GL_COLOR_BUFFER_BIT);

            for (Layer* layer : m_LayerStack)
            {
                layer->OnUpdate();
            }

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;

        return true;
    }
}