#include "Application.h"

#include "FileSystem.h"
#include "Input.h"
#include "SettingsConfig.h"
#include "Timer.h"
#include "Window.h"

#include "Renderer/OrthographicCameraController.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/TextRenderer.h"

#include "Resources/TextureManager.h"

#include "Events/EventManager.h"

#include "Scene/Components/SceneComponents.h"
#include "Scene/SceneManager.h"

#include <Resources/AudioManager.h>

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

    EL_CORE_INFO("Executable path: {0}", fileSystem::GetCurrentPath());

    // Init global engine settings
    gEngineSettings.LoadSettings();

    m_window = Window::Create({ "ElvenEngine", gEngineSettings.windowWidth, gEngineSettings.windowHeight, gEngineSettings.enableFullscreen, gEngineSettings.enableVSync });
    RenderCommand::SetViewport(0, 0, m_window->GetWidth(), m_window->GetHeight());

    // Create cameras entity before other inits, because this entity could be used by them
    auto& scene = gSceneManager.GetScene();
    m_orthoCameraEntity = scene.CreateEntity();

    // Init renderers
    elv::Renderer::Init();
    Renderer2D::Init();
    TextRenderer::Init();
    gSceneManager.Init();
    gAudioManager.Init();

#if EDITOR_MODE
    m_imGuiOverlay.Init();
#endif

    events::Subscribe<events::WindowCloseEvent>(m_windowCloseCallback);
    events::Subscribe<events::WindowResizeEvent>(m_windowResizeCallback);

    auto& cameraComponent = scene.AddComponent<CameraComponent>(m_orthoCameraEntity, false);
    const float aspectRatio = static_cast<float>(gEngineSettings.windowWidth) / static_cast<float>(gEngineSettings.windowHeight);
    cameraComponent.camera.SetProjection(-aspectRatio * gEngineSettings.orthographicCameraSize, aspectRatio * gEngineSettings.orthographicCameraSize, -gEngineSettings.orthographicCameraSize, gEngineSettings.orthographicCameraSize, -10.0f, 10.0f);

    // fps counter
    if (gEngineSettings.enableFpsCounter) {
        m_fpsCounterEntityId = scene.CreateEntity();
        scene.AddComponent<elv::RectTransformComponent>(m_fpsCounterEntityId, lia::vec2(0.0f, 100.0f), lia::vec2(0.6f, 0.6f));
        scene.AddComponent<elv::TextComponent>(m_fpsCounterEntityId, "0");
    }
}

Application::~Application()
{
    OnDestroy();

#if EDITOR_MODE
    m_imGuiOverlay.Shutdown();
#endif

    gAudioManager.Shutdown();
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

    Timer timerFpsCounter; // to update fps counter

    while (m_running) {

        if (m_isPaused) {
            // Window update only to catch maximized event
            m_window->OnUpdate();
        }

        const float elapsedTime = timer.Elapsed();
        timer.Restart();

        if (timerFpsCounter.ElapsedMs() > 100.0f) {
            s_telemetry.frameTime = elapsedTime * 1000.0f;

            s_telemetry.fps = 1.0f / elapsedTime;

            timerFpsCounter.Restart();
        }

        if (gEngineSettings.enableFpsCounter) {
            GetScene().GetComponent<elv::TextComponent>(m_fpsCounterEntityId).text
                = fmt::format("{:0.3f} ms ({} FPS)", s_telemetry.frameTime, static_cast<int>(std::floor(s_telemetry.fps)));
        }

        /// Update step ////////////////////

        gTextureManager.OnUpdate();
        gSceneManager.Update(elapsedTime);

        OnUpdate(elapsedTime);

        /////////////////////////////////////

        /// Rendering Step ///////////////////
        elv::RenderCommand::SetClearColor({ 0.2f, 0.2f, 0.2f, 1.0f });
        elv::RenderCommand::Clear();

        OnRender(elapsedTime);
        gSceneManager.Render(elapsedTime);

#if EDITOR_MODE
        if (gEngineSettings.enableEditor) {
            // ImGui overlay rendering
            m_imGuiOverlay.Begin();
            m_imGuiOverlay.ImGuiRender();
            OnImguiRender();
            m_imGuiOverlay.End();
        }
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
        m_isPaused = true;
        return;
    }
    m_isPaused = false;

    Renderer::OnWindowResize(e.width, e.height);

    if (!OrthographicCameraController::IsCustomCameraController()) {
        auto& cameraComponent = GetScene().GetComponent<CameraComponent>(m_orthoCameraEntity);
        const float aspectRatio = static_cast<float>(e.width) / static_cast<float>(e.height);

        auto& camera = GetScene().GetComponent<CameraComponent>(m_orthoCameraEntity).camera;
        camera.SetProjection(-aspectRatio * gEngineSettings.orthographicCameraSize,
                             aspectRatio * gEngineSettings.orthographicCameraSize,
                             -gEngineSettings.orthographicCameraSize,
                             gEngineSettings.orthographicCameraSize, -1.0f, 1.0f);
    }

    OnWindowResizeApp();
}

} // namespace elv
