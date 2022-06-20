#pragma once

#include "Core/LayerStack.h"
#include "Events/ApplicationEvent.h"

#include "Events/EventManager.h"

namespace Elven {

class Window;
class Layer;
class ImGuiLayer;

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

    Window& GetWindow() { return *m_window; }

    static Application& Get() { return *s_instance; }
    static Telemetry& GetTelemetry() { return s_telemetry; };

private:
    void OnWindowClose(const Events::WindowCloseEvent& e);
    void OnWindowResize(const Events::WindowResizeEvent& e);

private:
    static Application* s_instance;
    static Telemetry s_telemetry;

    Window* m_window { nullptr };
    ImGuiLayer* m_imGuiLayer { nullptr };
    LayerStack m_layerStack;

    Events::EventFunctionHandler<Events::WindowResizeEvent> m_windowResizeCallback;
    Events::EventFunctionHandler<Events::WindowCloseEvent> m_windowCloseCallback;

    bool m_running = false;
};

// To be defined in CLIENT
Application* CreateApplication();

} // namespace Elven
