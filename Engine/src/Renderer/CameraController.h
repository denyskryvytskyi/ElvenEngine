#pragma once

#include "Camera.h"

#include "Events/ApplicationEvent.h"
#include "Events/EventManager.h"
#include "Events/MouseEvent.h"

namespace Elven {

class CameraController {
public:
    CameraController(float fov, float aspectRatio, float near_, float far_, bool enableFly = false);
    ~CameraController();

    void OnUpdate(float dt);

    Camera& GetCamera() { return m_camera; }
    const Camera& GetCamera() const { return m_camera; }

private:
    void ProcessInput(float dt);

    void OnMouseMoved(const Events::MouseMovedEvent& e);
    void OnMouseScrolled(const Events::MouseScrolledEvent& e);
    void OnWindowResized(const Events::WindowResizeEvent& e);

private:
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
    bool m_updateLookAt { true };

    Events::EventFunctionHandler<Events::MouseMovedEvent> m_mouseMovedCallback;
    Events::EventFunctionHandler<Events::MouseScrolledEvent> m_mouseScrolledCallback;
    Events::EventFunctionHandler<Events::WindowResizeEvent> m_windowResizeCallback;
};

} // namespace Elven
