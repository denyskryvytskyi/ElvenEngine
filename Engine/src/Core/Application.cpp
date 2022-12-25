#include "Application.h"

#include "FileSystem.h"
#include "Input.h"
#include "SettingsConfig.h"
#include "Timer.h"
#include "Window.h"

#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/TextureManager.h"

#include "Events/EventManager.h"
#include "Scene/SceneManager.h"

namespace Elven {

Application* Application::s_instance = nullptr;
Application::Telemetry Application::s_telemetry;

Application::Application()
    : m_running(true)
    , m_windowCloseCallback([this](const events::WindowCloseEvent& e) { OnWindowClose(e); })
    , m_windowResizeCallback([this](const events::WindowResizeEvent& e) { OnWindowResize(e); })
{
    EL_CORE_ASSERT(!s_instance, "Application already exists!");
    s_instance = this;

    EL_CORE_INFO("Executable path: {0}", FileSystem::GetCurrentPath());

    // Init global engine settings
    gEngineSettings.LoadSettings();

    m_window = Window::Create({ "ElvenEngine", gEngineSettings.WindowWidth, gEngineSettings.WindowHeight });
    RenderCommand::SetViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());

    // Init managers here
    Elven::Renderer::Init();
    gSceneManager.Init();
    //

    m_imGuiOverlay.Init();

    events::Subscribe<events::WindowCloseEvent>(m_windowCloseCallback);
    events::Subscribe<events::WindowResizeEvent>(m_windowResizeCallback);

    OnCreate();
}

Application::~Application()
{
    OnDestroy();

    m_imGuiOverlay.Shutdown();
    gSceneManager.Shutdown();
    gTextureManager.Shutdown();
    events::gEventManager.Shutdown();
    Renderer::Shutdown();
    Renderer2D::Shutdown();
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

        OnUpdate(elapsedTime);

        gTextureManager.OnUpdate();
        gSceneManager.Update(elapsedTime);

        /////////////////////////////////////

        /// Rendering Step ///////////////////
        Elven::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
        Elven::RenderCommand::Clear();

        OnRender(elapsedTime);
        gSceneManager.Render(elapsedTime);

        // ImGui overlay rendering
        m_imGuiOverlay.Begin();
        m_imGuiOverlay.ImGuiRender();
        m_imGuiOverlay.End();

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
