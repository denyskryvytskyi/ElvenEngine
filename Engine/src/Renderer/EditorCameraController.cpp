#include "EditorCameraController.h"

#include "Core/Input.h"
#include "Events/EventManager.h"

namespace elv {

const static float s_cameraUpSpeed = 5.0f;
const static float s_cameraDownSpeed = 5.0f;
const static float s_cameraRotationSpeed = 50.0f;

EditorCameraController::EditorCameraController(float fov, float aspectRatio, float near_, float far_, bool enableFly)
    : CameraController(fov, aspectRatio, near_, far_, enableFly)
    , m_mouseButtonPressedCallback([this](const events::MouseButtonPressedEvent& e) { OnMouseButtonPressed(e); })
    , m_mouseButtonReleaseCallback([this](const events::MouseButtonReleasedEvent& e) { OnMouseButtonRelease(e); })
{
    m_isMoveEnabled = false;
    events::Subscribe<events::MouseButtonPressedEvent>(m_mouseButtonPressedCallback);
    events::Subscribe<events::MouseButtonReleasedEvent>(m_mouseButtonReleaseCallback);
}

void EditorCameraController::ProcessInput(float dt)
{
    CameraController::ProcessInput(dt);

    if (m_isMoveEnabled) {
        if (Input::IsKeyPressed(key::E)) {
            m_position.y += s_cameraUpSpeed * dt;
            m_updateLookAt = true;
        }

        if (Input::IsKeyPressed(key::Q)) {
            m_position.y -= s_cameraDownSpeed * dt;
            m_updateLookAt = true;
        }

        if (Input::IsKeyPressed(key::Z)) {
            m_yaw -= s_cameraRotationSpeed * dt;
            UpdateFront();
        }

        if (Input::IsKeyPressed(key::X)) {
            m_yaw += s_cameraRotationSpeed * dt;
            UpdateFront();
        }
    }
}

void EditorCameraController::OnMouseScrolled(const events::MouseScrolledEvent& e)
{
    if (m_isMoveEnabled) {
        m_cameraMovementSpeed += e.yOffset;
    }
}

void EditorCameraController::OnMouseButtonPressed(const events::MouseButtonPressedEvent& e)
{
    if (e.button == Mouse::ButtonRight) {
        m_isMoveEnabled = true;
    }
}

void EditorCameraController::OnMouseButtonRelease(const events::MouseButtonReleasedEvent& e)
{
    if (e.button == Mouse::ButtonRight) {
        m_isMoveEnabled = false;
        m_mouseLastPosInited = false;
    }
}

} // namespace elv
