#include "Application.h"

#include "FileSystem.h"
#include "Input.h"
#include "SettingsConfig.h"
#include "Timer.h"
#include "Window.h"

#include "Renderer/CameraController.h"
#include "Renderer/OrthographicCameraController.h"
#include "Renderer/Renderer.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/TextRenderer.h"

#include "Resources/MeshLibrary.h"
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

    gTextureManager.Init();
    gMeshLibrary.Init();
    Renderer::Init();
    Renderer2D::Init();
    TextRenderer::Init();
    gAudioManager.Init();
    gSceneManager.Init();

#if EDITOR_MODE
    m_imGuiOverlay.Init();
    m_editor.OnInit();
#endif

    events::Subscribe<events::WindowCloseEvent>(m_windowCloseCallback);
    events::Subscribe<events::WindowResizeEvent>(m_windowResizeCallback);

    // orthographic camera
    auto& scene = gSceneManager.GetScene();
    m_orthoCameraEntity = scene.CreateEntity();
    scene.AddComponent<TagComponent>(m_orthoCameraEntity, "Ortho camera");
    auto& cameraComponent = scene.AddComponent<CameraComponent>(m_orthoCameraEntity, false);
    const float aspectRatio = static_cast<float>(gEngineSettings.windowWidth) / static_cast<float>(gEngineSettings.windowHeight);
    cameraComponent.camera.SetProjection(-aspectRatio * gEngineSettings.orthographicCameraSize, aspectRatio * gEngineSettings.orthographicCameraSize, -gEngineSettings.orthographicCameraSize, gEngineSettings.orthographicCameraSize, -10.0f, 10.0f);

    // fps counter
    if (gEngineSettings.enableFpsCounter) {
        m_fpsCounterEntityId = scene.CreateEntity();
        scene.AddComponent<TagComponent>(m_fpsCounterEntityId, "FPS text");
        scene.AddComponent<RectTransformComponent>(m_fpsCounterEntityId, lia::vec2(0.5f, 97.0f), lia::vec2(0.4f, 0.4f));
        scene.AddComponent<TextComponent>(m_fpsCounterEntityId, "0");
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
    gMeshLibrary.Shutdown();
    gSceneManager.Shutdown();
    gTextureManager.Shutdown();
    Renderer::Shutdown();
    Renderer2D::Shutdown();
}

void Application::Run()
{
    OnCreate();

    Timer timer;
    Timer timerFpsCounter; // for fps counter update

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
            GetScene().GetComponent<TextComponent>(m_fpsCounterEntityId).text
                = fmt::format("{:0.3f} ms ({} fps)", s_telemetry.frameTime, static_cast<int>(std::floor(s_telemetry.fps)));
        }

        // ============== Process input step  ==============

        if (m_cameraController) {
            m_cameraController->OnProcessInput(elapsedTime);
        }
        OnProcessInput(elapsedTime);

        // ============== Update step  ==============

        if (m_cameraController) {
            m_cameraController->OnUpdate(elapsedTime);
        }
        gMeshLibrary.Update();
        gTextureManager.Update();
        gSceneManager.Update(elapsedTime);

        OnUpdate(elapsedTime);

        // ============== Rendering Step ==============
        RenderCommand::SetClearColor(Renderer::GetClearColor());
        RenderCommand::Clear();

        gSceneManager.Render(elapsedTime);
        OnRender(elapsedTime);

#if EDITOR_MODE
        if (gEngineSettings.enableEditor) {
            m_imGuiOverlay.Begin();
            m_editor.OnImGuiRender();
            OnImguiRender();
            m_imGuiOverlay.End();
        }
#endif

        // ============== Window update step ==============
        m_window->OnUpdate();

        // ============== Dispatch queued events ==============
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

        auto& camera = cameraComponent.camera;
        camera.SetProjection(-aspectRatio * gEngineSettings.orthographicCameraSize,
                             aspectRatio * gEngineSettings.orthographicCameraSize,
                             -gEngineSettings.orthographicCameraSize,
                             gEngineSettings.orthographicCameraSize, -1.0f, 1.0f);
    }

    OnWindowResizeApp();
}

} // namespace elv
