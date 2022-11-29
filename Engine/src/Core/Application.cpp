#include "Application.h"
#include "FileSystem.h"
#include "Input.h"
#include "Timer.h"

#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/TextureManager.h"

#include "Events/EventManager.h"
#include "ImGui/ImGuiLayer.h"

namespace Elven {

Application* Application::s_instance = nullptr;
Application::Telemetry Application::s_telemetry;

Application::Application()
    : m_running(true)
    , m_imGuiLayer(new ImGuiLayer)
    , m_windowCloseCallback([this](const events::WindowCloseEvent& e) { OnWindowClose(e); })
    , m_windowResizeCallback([this](const events::WindowResizeEvent& e) { OnWindowResize(e); })
{
    EL_CORE_ASSERT(!s_instance, "Application already exists!");
    s_instance = this;

    PushOverlay(m_imGuiLayer);

    // TODO: Make it dependent from user settings to init Renderer or Renderer2D (add option to menu)
    // Elven::Renderer::Init();

    events::Subscribe<events::WindowCloseEvent>(m_windowCloseCallback);
    events::Subscribe<events::WindowResizeEvent>(m_windowResizeCallback);

    EL_INFO("Executable path: {0}", FileSystem::GetCurrentPath());

    Elven::Renderer::Init();
    m_scene.Init();
}

Application::~Application()
{
    events::Unsubscribe<events::WindowResizeEvent>(m_windowResizeCallback);
    events::Unsubscribe<events::WindowCloseEvent>(m_windowCloseCallback);

    gTextureManager.Shutdown();
    events::gEventManager.Shutdown();
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
        const float elapsedTime = timer.Elapsed();
        timer.Restart();

        s_telemetry.frameTime = elapsedTime * 1000.0f;
        s_telemetry.fps = 1.0f / elapsedTime;

        /// Update step ////////////////////
        gTextureManager.OnUpdate();

        // Layers update
        for (Layer* layer : m_layerStack) {
            layer->OnUpdate(elapsedTime);
        }

        m_scene.OnUpdate(elapsedTime);

        /////////////////////////////////////

        /// Rendering Step ///////////////////
        m_scene.OnRender(elapsedTime);

        // ImGui layers render
        m_imGuiLayer->Begin();
        for (Layer* layer : m_layerStack) {
            layer->OnImGuiRender();
        }
        m_imGuiLayer->End();

        // Window update
        m_window->OnUpdate();

        /////////////////////////////////////

        // Dispatch queued events
        events::gEventManager.DispatchEvents();
    }
}

void Application::OnWindowClose(const events::WindowCloseEvent& e)
{
    m_running = false;
}

void Application::OnWindowResize(const events::WindowResizeEvent& e)
{
    if (e.width == 0 || e.height == 0) {
        // minimized logic
    }

    Renderer::OnWindowResize(e.width, e.height);
}

} // namespace Elven
