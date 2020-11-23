#include "elpch.h"

#include "Elven/Core/Application.h"
#include "Elven/Core/Input.h"

#include "Elven/Renderer/Buffer.h"
#include "Elven/Renderer/VertexArray.h"
#include "Elven/Renderer/Renderer.h"

namespace Elven
{
    Ref<VertexArray> VA;

    Application* Application::s_Instance = nullptr;

    Application::Application()
        : m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
    {
        EL_CORE_ASSERT(!s_Instance, "Application already exist!");
        s_Instance = this;

        m_Window = Window::Create();
        m_Window->SetEventCallback(EL_BIND_EVENT_FN(Application::OnEvent));

        m_ImGuiLayer = new ImGuiLayer();
        PushOverlay(m_ImGuiLayer);

        float vertices[] = {
            -0.5f, -0.5f, 0.0f, 0.1f, 0.2f, 0.6f, 1.0f,
            -0.5f, 0.5f, 0.0f, 0.6f, 0.2f, 0.2f, 1.0f,
            0.5f, 0.5f, 0.0f, 0.1f, 0.7f, 0.1f, 1.0f,
            0.5f, -0.5f, 0.0f, 0.5f, 0.5f, 0.5f, 1.0f
        };

        VA = VertexArray::Create();

        Ref<VertexBuffer> VBO = VertexBuffer::Create(vertices, sizeof(vertices));
        VertexBufferLayout layout = {
            {BufferAttributeType::Float3, "a_Position" },
            {BufferAttributeType::Float4, "a_Color" }
        };
        VBO->SetLayout(layout);

        VA->AddVertexBuffer(VBO);

        uint32_t indices[] = {
            0, 1, 2, 0, 2, 3
        };
        Ref<IndexBuffer> IBO = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));

        VA->SetIndexBuffer(IBO);

        shaderManager.Load("test", "res/shaders/shader.vert", "res/shaders/shader.frag");
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event& e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(EL_BIND_EVENT_FN(Application::OnWindowClose));

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
            RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
            RenderCommand::Clear();

            Renderer::BeginScene(m_Camera);

            shaderManager.Get("test")->Bind();

            m_Camera.SetPosition(gdm::vec3(0.5f, 0.5f, 0.0f));
            m_Camera.SetRotation(65.0f);

            Renderer::Submit(shaderManager.Get("test"), VA);

            // Layers update
            for (Layer* layer : m_LayerStack)
            {
                layer->OnUpdate();
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
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;

        return true;
    }
}