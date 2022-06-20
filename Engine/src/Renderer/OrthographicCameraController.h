#pragma once

#include "Events/ApplicationEvent.h"
#include "Events/MouseEvent.h"
#include "Renderer/Camera.h"

#include "Events/EventManager.h"

namespace Elven {

class OrthographicCameraController {
public:
    OrthographicCameraController(float aspectRatio);
    ~OrthographicCameraController();

    void OnUpdate(float dt);

    Camera& GetCamera() { return m_camera; }
    const Camera& GetCamera() const { return m_camera; }

    void SetZoomLevel(float zoomLevel) { m_zoomLevel = zoomLevel; }
    float GetZoomLevel() const { return m_zoomLevel; }

private:
    void OnMouseScrolled(Events::MouseScrolledEvent& e);
    void OnWindowResized(Events::WindowResizeEvent& e);

private:
    lia::vec3 m_position;

    float m_aspectRatio { 0.0f };
    float m_zoomLevel { 1.0f };
    float m_rotation { 0.0f };
    float m_translationSpeed { 5.0f };
    float m_rotationSpeed { 180.0f };

    Camera m_camera;

    Events::EventFunctionHandler<Events::WindowResizeEvent> m_windowResizeCallback;
    Events::EventFunctionHandler<Events::MouseScrolledEvent> m_mouseScrolledCallback;
};

} // namespace Elven
