#include "OrthographicCameraController.h"

#include "Core/Application.h"
#include "Core/Input.h"
#include "Core/SettingsConfig.h"
#include "Events/EventManager.h"
#include "Scene/Components/SceneComponents.h"
#include "Scene/SceneManager.h"

namespace elv {

constexpr float defaultTranslationSpeed = 20.0f;

bool OrthographicCameraController::isCustomCameraController = false;

OrthographicCameraController::OrthographicCameraController(float aspectRatio)
    : m_aspectRatio(aspectRatio)
    , m_orthoCameraEntity(Application::Get().GetOrthographicCameraEntity())
    , m_windowResizeCallback([this](const events::WindowResizeEvent& e) { OnWindowResized(e); })
    , m_mouseScrolledCallback([this](const events::MouseScrolledEvent& e) { OnMouseScrolled(e); })
{
    events::Subscribe<events::WindowResizeEvent>(m_windowResizeCallback);
    events::Subscribe<events::MouseScrolledEvent>(m_mouseScrolledCallback);

    isCustomCameraController = true;
}

void OrthographicCameraController::OnUpdate(float dt)
{
    if (Input::IsKeyPressed(key::A)) {
        m_position.x -= cos(lia::radians(m_rotation)) * m_translationSpeed * dt;
        m_position.y -= sin(lia::radians(m_rotation)) * m_translationSpeed * dt;
    } else if (Input::IsKeyPressed(key::D)) {
        m_position.x += cos(lia::radians(m_rotation)) * m_translationSpeed * dt;
        m_position.y += sin(lia::radians(m_rotation)) * m_translationSpeed * dt;
    }

    if (Input::IsKeyPressed(key::W)) {
        m_position.x += -sin(lia::radians(m_rotation)) * m_translationSpeed * dt;
        m_position.y += cos(lia::radians(m_rotation)) * m_translationSpeed * dt;
    } else if (Input::IsKeyPressed(key::S)) {
        m_position.x -= -sin(lia::radians(m_rotation)) * m_translationSpeed * dt;
        m_position.y -= cos(lia::radians(m_rotation)) * m_translationSpeed * dt;
    }

    if (Input::IsKeyPressed(key::Q))
        m_rotation += m_rotationSpeed * dt;
    if (Input::IsKeyPressed(key::E))
        m_rotation -= m_rotationSpeed * dt;

    if (m_rotation > 180.0f)
        m_rotation -= 360.0f;
    else if (m_rotation <= -180.0f)
        m_rotation += 360.0f;

    auto& camera = GetScene().GetComponent<CameraComponent>(m_orthoCameraEntity).camera;
    camera.SetRotation({ 0.0f, 0.0f, m_rotation });
    camera.SetPosition(m_position);
}

void OrthographicCameraController::OnMouseScrolled(const events::MouseScrolledEvent& e)
{
    m_zoomLevel -= e.yOffset * m_zoomSpeed;
    m_zoomLevel = std::max<float>(m_zoomLevel, m_zoomSpeed);
    m_translationSpeed = defaultTranslationSpeed * m_zoomLevel;

    auto& camera = GetScene().GetComponent<CameraComponent>(m_orthoCameraEntity).camera;
    camera.SetProjection(-m_aspectRatio * m_zoomLevel * gEngineSettings.orthographicCameraSize,
                         m_aspectRatio * m_zoomLevel * gEngineSettings.orthographicCameraSize,
                         -m_zoomLevel * gEngineSettings.orthographicCameraSize,
                         m_zoomLevel * gEngineSettings.orthographicCameraSize, -1.0f, 1.0f);
}

void OrthographicCameraController::OnWindowResized(const events::WindowResizeEvent& e)
{
    m_aspectRatio = static_cast<float>(e.width) / static_cast<float>(e.height);

    auto& camera = GetScene().GetComponent<CameraComponent>(m_orthoCameraEntity).camera;
    camera.SetProjection(-m_aspectRatio * m_zoomLevel * gEngineSettings.orthographicCameraSize,
                         m_aspectRatio * m_zoomLevel * gEngineSettings.orthographicCameraSize,
                         -m_zoomLevel * gEngineSettings.orthographicCameraSize,
                         m_zoomLevel * gEngineSettings.orthographicCameraSize, -1.0f, 1.0f);
}

} // namespace elv
