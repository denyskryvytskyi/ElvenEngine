#include "Application.h"

#include "FileSystem.h"
#include "Input.h"
#include "Profiler.h"
#include "SettingsConfig.h"
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

    gEngineSettings.LoadSettings();

    m_window = Window::Create({ "ElvenEngine", gEngineSettings.windowWidth, gEngineSettings.windowHeight, gEngineSettings.enableFullscreen, gEngineSettings.enableVSync });

    gTextureManager.Init();
    gMeshLibrary.Init();
    m_renderer.Init(m_window->GetWidth(), m_window->GetHeight());
    Renderer2D::Init(&m_renderer);
    TextRenderer::Init(m_renderer);
    gAudioManager.Init();
    gSceneManager.Init();

#if EDITOR_MODE
    if (gEngineSettings.enableEditor) {
        m_imGuiOverlay.Init();
        m_editor.OnInit();
    }
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
        scene.AddComponent<UITransformComponent>(m_fpsCounterEntityId, lia::vec2(0.5f, 97.0f), lia::vec2(0.4f, 0.4f));
        scene.AddComponent<TextComponent>(m_fpsCounterEntityId, "0");
    }
}

Application::~Application()
{
    OnDestroy();

#if EDITOR_MODE
    if (gEngineSettings.enableEditor) {
        m_imGuiOverlay.Shutdown();
    }
#endif

    gAudioManager.Shutdown();
    events::gEventManager.Shutdown();
    gMeshLibrary.Shutdown();
    gSceneManager.Shutdown();
    gTextureManager.Shutdown();
    m_renderer.Shutdown();
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

        const float elapsedTimeMs = timer.ElapsedMs();
        const float elapsedTime = elapsedTimeMs * 0.001f;
        timer.Restart();

        UpdateFps(timerFpsCounter, elapsedTime, elapsedTimeMs);

        // ============== Process input step  ==============
        {
            PROFILE_SCOPE("Process input in: ");

            if (m_cameraController) {
                m_cameraController->OnProcessInput(elapsedTime);
            }
            OnProcessInput(elapsedTime);
        }

        // ============== Update step  ==============

        {
            PROFILE_SCOPE("Update input in: ");
            if (m_cameraController) {
                m_cameraController->OnUpdate(elapsedTime);
            }
            gMeshLibrary.Update();
            gTextureManager.Update();
            gSceneManager.Update(elapsedTime);

            OnUpdate(elapsedTime);
        }

        // ============== Dispatch queued events ==============
        {
            PROFILE_SCOPE("Dispatch events in: ");
            events::gEventManager.DispatchEvents();
        }

        // ============== Rendering Step ==============
        {
            PROFILE_SCOPE("Render in: ");
            m_renderer.BeginScene(m_cameraController);
            gSceneManager.Render(elapsedTime);
            OnRender(elapsedTime);
            m_renderer.EndScene();
        }

#if EDITOR_MODE
        if (gEngineSettings.enableEditor) {
            PROFILE_SCOPE("ImGui Render in: ");
            m_imGuiOverlay.Begin();
            m_editor.OnImGuiRender();
            OnImguiRender();
            m_imGuiOverlay.End();
        }
#endif

        // ============== Window update step ==============
        {
            PROFILE_SCOPE("Update window in: ");
            m_window->OnUpdate();
        }
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

    m_renderer.OnWindowResize(e.width, e.height);

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

void Application::UpdateFps(Timer& fpsCounter, const float elapsed, const float elapsedMs)
{
    static float accumTime = 0.0f;
    static float accumTimeMs = 0.0f;
    static float accumFrameCount = 0.0f;

    if (fpsCounter.ElapsedMs() > 1000.0f) {
        s_telemetry.frameTime = accumTimeMs / accumFrameCount;
        s_telemetry.fps = accumFrameCount / accumTime;

        fpsCounter.Restart();
        accumTime = 0.0f;
        accumTimeMs = 0.0f;
        accumFrameCount = 0.0f;

        if (gEngineSettings.enableFpsCounter) {
            GetScene().GetComponent<TextComponent>(m_fpsCounterEntityId).text
                = fmt::format("{:0.3f} ms ({} fps)", s_telemetry.frameTime, static_cast<int>(std::floor(s_telemetry.fps)));
        }
    } else {
        accumTime += elapsed;
        accumTimeMs += elapsedMs;
        accumFrameCount += 1.0f;
    }
}

} // namespace elv
