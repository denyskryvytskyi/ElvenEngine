#include "Core/Application.h"
#include "Core/Input.h"
#include "Core/Timer.h"
#include "Core/Window.h"

#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"

#include "FileSystem.h"
#include "ImGui/ImGuiLayer.h"

namespace Elven {

Application* Application::s_instance = nullptr;
Application::Telemetry Application::s_telemetry;

Application::Application()
    : m_running(true)
    , m_imGuiLayer(new ImGuiLayer)
    , m_windowCloseCallback([this](const Events::WindowCloseEvent& e) { OnWindowClose(e); })
    , m_windowResizeCallback([this](const Events::WindowResizeEvent& e) { OnWindowResize(e); })
{
    EL_CORE_ASSERT(!s_instance, "Application already exists!");
    s_instance = this;

    PushOverlay(m_imGuiLayer);

    // TODO: Make it dependent from user settings to init Renderer or Renderer2D (add option to menu)
    // Elven::Renderer::Init();

    Events::Subscribe<Events::WindowCloseEvent>(m_windowCloseCallback);
    Events::Subscribe<Events::WindowResizeEvent>(m_windowResizeCallback);

    EL_INFO("Executable path: {0}", FileSystem::GetCurrentPath());
}

Application::~Application()
{
    Events::Unsubscribe<Events::WindowResizeEvent>(m_windowResizeCallback);
    Events::Unsubscribe<Events::WindowCloseEvent>(m_windowCloseCallback);

    Events::gEventManager.Shutdown();
    Renderer::Shutdown();
    Renderer2D::Shutdown();
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
    while (m_running) {
        float elapsedTime = timer.elapsed();
        timer.restart();

        s_telemetry.frameTime = elapsedTime * 1000.0f;
        s_telemetry.fps = 1.0f / elapsedTime;

        // Layers update
        for (Layer* layer : m_layerStack) {
            layer->OnUpdate(elapsedTime);
        }

        // ImGui layers render
        m_imGuiLayer->Begin();
        for (Layer* layer : m_layerStack) {
            layer->OnImGuiRender();
        }
        m_imGuiLayer->End();

        // Window update
        m_window->OnUpdate();

        // Dispatch queued events
        Events::gEventManager.DispatchEvents();
    }
}

void Application::OnWindowClose(const Events::WindowCloseEvent& e)
{
    m_running = false;
}

void Application::OnWindowResize(const Events::WindowResizeEvent& e)
{
    if (e.Width == 0 || e.Height == 0) {
        // minimized logic
    }

    Renderer::OnWindowResize(e.Width, e.Height);
}

} // namespace Elven
