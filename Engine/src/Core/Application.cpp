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

#include "Scene/Components/SceneComponents.h"
#include "Scene/SceneManager.h"

namespace elv {

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

    m_window = Window::Create({ "ElvenEngine", gEngineSettings.windowWidth, gEngineSettings.windowHeight });
    RenderCommand::SetViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());

    // Create cameras entity before other inits, because this entity could be used by them
    auto& scene = gSceneManager.GetScene();
    m_orthoCameraEntity = scene.CreateEntity();

    // Init managers here
    elv::Renderer::Init();
    Renderer2D::Init();

    gSceneManager.Init();
    //

#if EDITOR_MODE
    m_imGuiOverlay.Init();
#endif

    events::Subscribe<events::WindowCloseEvent>(m_windowCloseCallback);
    events::Subscribe<events::WindowResizeEvent>(m_windowResizeCallback);

    // Add real cameras
    auto& cameraComponent = scene.AddComponent<CameraComponent>(m_orthoCameraEntity, false);
    const float aspectRatio = static_cast<float>(gEngineSettings.windowWidth) / static_cast<float>(gEngineSettings.windowHeight);
    cameraComponent.camera.SetProjection(-aspectRatio * gEngineSettings.orthographicCameraSize, aspectRatio * gEngineSettings.orthographicCameraSize, -gEngineSettings.orthographicCameraSize, gEngineSettings.orthographicCameraSize, -1.0f, 1.0f);
}

Application::~Application()
{
    OnDestroy();

#if EDITOR_MODE
    m_imGuiOverlay.Shutdown();
#endif

    events::gEventManager.Shutdown();
    gSceneManager.Shutdown();
    gTextureManager.Shutdown();
    Renderer::Shutdown();
    Renderer2D::Shutdown();
}

void Application::Run()
{
    OnCreate();

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
        elv::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
        elv::RenderCommand::Clear();

        OnRender(elapsedTime);
        gSceneManager.Render(elapsedTime);

#if EDITOR_MODE
        // ImGui overlay rendering
        m_imGuiOverlay.Begin();
        m_imGuiOverlay.ImGuiRender();
        m_imGuiOverlay.End();
#endif

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

} // namespace elv
