#pragma once

#include "Core/LayerStack.h"
#include "Events/ApplicationEvent.h"
#include "Events/EventManager.h"
#include "Scene/Scene.h"
#include "Window.h"

namespace Elven {

class Window;
class Layer;
class ImGuiLayer;
class Scene;

class Application {
private:
    struct Telemetry {
        float frameTime { 0.0f };
        float fps { 0.0f };
    };

public:
    Application();
    virtual ~Application();

    void PushLayer(Layer* layer);
    void PushOverlay(Layer* overlay);

    void Run();

    UniquePtr<Window>& GetWindow() { return m_window; }

    static Application& Get() { return *s_instance; }
    static Telemetry& GetTelemetry() { return s_telemetry; };

private:
    void OnWindowClose(const events::WindowCloseEvent& e);
    void OnWindowResize(const events::WindowResizeEvent& e);

private:
    static Application* s_instance;
    static Telemetry s_telemetry;

    bool m_running { false };

    UniquePtr<Window> m_window { Window::Create() };
    ImGuiLayer* m_imGuiLayer { nullptr };
    LayerStack m_layerStack;
    Scene m_scene;

    events::EventFunctionHandler<events::WindowResizeEvent> m_windowResizeCallback;
    events::EventFunctionHandler<events::WindowCloseEvent> m_windowCloseCallback;
};

// To be defined in CLIENT
Application* CreateApplication();

} // namespace Elven
