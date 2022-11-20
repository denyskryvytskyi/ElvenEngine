#include "OrthographicCameraController.h"

#include "Core/Input.h"

namespace Elven {

OrthographicCameraController::OrthographicCameraController(float aspectRatio)
    : m_camera(-aspectRatio * m_zoomLevel, aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel, -1.0f, 1.0f)
    , m_aspectRatio(aspectRatio)
    , m_windowResizeCallback([this](const Events::WindowResizeEvent& e) { OnWindowResized(e); })
    , m_mouseScrolledCallback([this](const Events::MouseScrolledEvent& e) { OnMouseScrolled(e); })
{
    Events::Subscribe<Events::WindowResizeEvent>(m_windowResizeCallback);
    Events::Subscribe<Events::MouseScrolledEvent>(m_mouseScrolledCallback);
}

OrthographicCameraController::~OrthographicCameraController()
{
    Events::Unsubscribe<Events::MouseScrolledEvent>(m_mouseScrolledCallback);
    Events::Unsubscribe<Events::WindowResizeEvent>(m_windowResizeCallback);
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

void OrthographicCameraController::OnMouseScrolled(const Events::MouseScrolledEvent& e)
{
    m_zoomLevel -= e.YOffset * 0.25f;
    m_zoomLevel = std::max<float>(m_zoomLevel, 0.25f);
    m_camera.SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel, -1.0f, 1.0f);
}

void OrthographicCameraController::OnWindowResized(const Events::WindowResizeEvent& e)
{
    m_aspectRatio = static_cast<float>(e.Width) / static_cast<float>(e.Height);
    m_camera.SetProjection(-m_aspectRatio * m_zoomLevel, m_aspectRatio * m_zoomLevel, -m_zoomLevel, m_zoomLevel, -1.0f, 1.0f);
}
} // namespace Elven
