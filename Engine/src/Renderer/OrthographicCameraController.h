#pragma once

#include "Events/ApplicationEvent.h"
#include "Events/EventHandler.h"
#include "Events/MouseEvent.h"

#include "Renderer/Camera.h"

#include "Scene/Entity.h"

namespace elv {

class OrthographicCameraController {
public:
    OrthographicCameraController(float aspectRatio);

    void OnUpdate(float dt);

    void SetZoomLevel(float zoomLevel) { m_zoomLevel = zoomLevel; }
    float GetZoomLevel() const { return m_zoomLevel; }

    lia::vec4 GetCameraBounds() const
    {
        return m_bounds;
    }

    static bool IsCustomCameraController() { return isCustomCameraController; }

private:
    void OnMouseScrolled(const events::MouseScrolledEvent& e);
    void OnWindowResized(const events::WindowResizeEvent& e);

private:
    lia::vec4 m_bounds;
    lia::vec3 m_position;

    float m_aspectRatio { 0.0f };
    float m_zoomLevel { 1.0f };
    float m_rotation { 0.0f };

    float m_translationSpeed { 20.0f };
    float m_zoomSpeed { 0.25f };
    float m_rotationSpeed { 180.0f };

    static bool isCustomCameraController;

    ecs::Entity m_orthoCameraEntity { ecs::INVALID_ENTITY_ID };

    events::EventHandler<events::WindowResizeEvent> m_windowResizeCallback;
    events::EventHandler<events::MouseScrolledEvent> m_mouseScrolledCallback;
};

} // namespace elv
