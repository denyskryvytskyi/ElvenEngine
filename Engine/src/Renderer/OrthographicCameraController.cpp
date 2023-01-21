#include "OrthographicCameraController.h"

#include "Core/Input.h"
#include "Events/EventManager.h"

namespace elv {

constexpr int cameraSize = 10;

OrthographicCameraController::OrthographicCameraController(float aspectRatio)
    : m_camera(-m_aspectRatio * m_zoomLevel * cameraSize, m_aspectRatio * m_zoomLevel * cameraSize, -m_zoomLevel * cameraSize, m_zoomLevel * cameraSize, -1.0f, 1.0f)
    , m_aspectRatio(aspectRatio)
    , m_windowResizeCallback([this](const events::WindowResizeEvent& e) { OnWindowResized(e); })
    , m_mouseScrolledCallback([this](const events::MouseScrolledEvent& e) { OnMouseScrolled(e); })
{
    events::Subscribe<events::WindowResizeEvent>(m_windowResizeCallback);
    events::Subscribe<events::MouseScrolledEvent>(m_mouseScrolledCallback);
}

void OrthographicCameraController::OnUpdate(float dt)
{
    if (Input::IsKeyPressed(Key::A)) {
        m_position.x -= cos(lia::radians(m_rotation)) * m_translationSpeed * dt;
        m_position.y -= sin(lia::radians(m_rotation)) * m_translationSpeed * dt;
    } else if (Input::IsKeyPressed(Key::D)) {
        m_position.x += cos(lia::radians(m_rotation)) * m_translationSpeed * dt;
        m_position.y += sin(lia::radians(m_rotation)) * m_translationSpeed * dt;
    }

    if (Input::IsKeyPressed(Key::W)) {
        m_position.x += -sin(lia::radians(m_rotation)) * m_translationSpeed * dt;
        m_position.y += cos(lia::radians(m_rotation)) * m_translationSpeed * dt;
    } else if (Input::IsKeyPressed(Key::S)) {
        m_position.x -= -sin(lia::radians(m_rotation)) * m_translationSpeed * dt;
        m_position.y -= cos(lia::radians(m_rotation)) * m_translationSpeed * dt;
    }

    if (Input::IsKeyPressed(Key::Q))
        m_rotation += m_rotationSpeed * dt;
    if (Input::IsKeyPressed(Key::E))
        m_rotation -= m_rotationSpeed * dt;

    if (m_rotation > 180.0f)
        m_rotation -= 360.0f;
    else if (m_rotation <= -180.0f)
        m_rotation += 360.0f;

    m_camera.SetRotation({ 0.0f, 0.0f, m_rotation });
    m_camera.SetPosition(m_position);

    m_translationSpeed = m_zoomLevel;
}

void OrthographicCameraController::OnMouseScrolled(const events::MouseScrolledEvent& e)
{
    m_zoomLevel -= e.yOffset * 0.25f;
    m_zoomLevel = std::max<float>(m_zoomLevel, 0.25f);
    m_camera.SetProjection(-m_aspectRatio * m_zoomLevel * cameraSize, m_aspectRatio * m_zoomLevel * cameraSize, -m_zoomLevel * cameraSize, m_zoomLevel * cameraSize, -1.0f, 1.0f);
}

void OrthographicCameraController::OnWindowResized(const events::WindowResizeEvent& e)
{
    m_aspectRatio = static_cast<float>(e.width) / static_cast<float>(e.height);
    m_camera.SetProjection(-m_aspectRatio * m_zoomLevel * 10, m_aspectRatio * m_zoomLevel * 10, -m_zoomLevel * cameraSize, m_zoomLevel * cameraSize, -1.0f, 1.0f);
}
} // namespace elv
