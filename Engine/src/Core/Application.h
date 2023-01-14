#pragma once

#include "Events/ApplicationEvent.h"
#include "Events/EventHandler.h"
#include "ImGui/ImGuiOverlay.h"

namespace elv {

class Window;
class Application {
private:
    struct Telemetry {
        float frameTime { 0.0f };
        float fps { 0.0f };
    };

public:
    Application();
    virtual ~Application();
    Application(const Application&) = delete;
    const Application& operator=(const Application&) = delete;

    void Run();

    UniquePtr<Window>& GetWindow() { return m_window; }

    static Application& Get() { return *s_instance; }
    static Telemetry& GetTelemetry() { return s_telemetry; };

protected:
    virtual void OnCreate() {};
    virtual void OnUpdate(float dt) {};
    virtual void OnRender(float dt) {};
    virtual void OnDestroy() {};

private:
    void OnWindowClose(const events::WindowCloseEvent& e);
    void OnWindowResize(const events::WindowResizeEvent& e);

private:
    static Application* s_instance;
    static Telemetry s_telemetry;

    bool m_running { false };

    UniquePtr<Window> m_window { nullptr };
    ImGuiOverlay m_imGuiOverlay;

    events::EventHandler<events::WindowResizeEvent> m_windowResizeCallback;
    events::EventHandler<events::WindowCloseEvent> m_windowCloseCallback;
};

// To be defined in CLIENT
Application* CreateApplication();

} // namespace elv
