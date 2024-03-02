#pragma once

#include "Camera.h"

#include "Events/ApplicationEvent.h"
#include "Events/EventHandler.h"
#include "Events/MouseEvent.h"

namespace elv {

class CameraController {
public:
    CameraController(float fov, float aspectRatio, float near_, float far_, bool enableFly = false);
    virtual ~CameraController() = default;

    void OnUpdate(float dt);

    Camera& GetCamera() { return m_camera; }
    const Camera& GetCamera() const { return m_camera; }

    const lia::vec3& GetFront() const { return m_front; };

protected:
    virtual void ProcessInput(float dt);
    virtual void OnMouseMoved(const events::MouseMovedEvent& e);

    void UpdateFront();

private:
    void OnMouseScrolled(const events::MouseScrolledEvent& e);
    void OnWindowResized(const events::WindowResizeEvent& e);

protected:
    Camera m_camera;

    lia::vec3 m_position { 0.0f, 0.0f, 3.0f };
    lia::vec3 m_front { 0.0f, 0.0f, -1.0f };
    lia::vec3 m_up { 0.0f, 1.0f, 0.0f };

    lia::vec2 m_mouseLastPos;

    float m_fov { 0.0f };
    float m_aspectRatio { 0.0f };
    float m_near { 0.0f };
    float m_far { 0.0f };

    float m_yaw { -90.0f };
    float m_pitch { 0.0f };

    bool m_mouseLastPosInited { false };
    bool m_isFlyEnabled { false };
    bool m_isMoveEnabled { true };
    bool m_updateLookAt { true };

    events::EventHandler<events::MouseMovedEvent> m_mouseMovedCallback;
    events::EventHandler<events::MouseScrolledEvent> m_mouseScrolledCallback;
    events::EventHandler<events::WindowResizeEvent> m_windowResizeCallback;
};

} // namespace elv
