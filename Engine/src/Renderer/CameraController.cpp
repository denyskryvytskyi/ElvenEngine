#include "CameraController.h"

#include "Core/Input.h"

namespace Elven {

const static float s_cameraMovementSpeed = 5.0f;

CameraController::CameraController(float fov, float aspectRatio, float near_, float far_, bool enableFly)
    : m_camera(lia::radians(fov), aspectRatio, near_, far_)
    , m_fov(fov)
    , m_aspectRatio(aspectRatio)
    , m_near(near_)
    , m_far(far_)
    , m_isFlyEnabled(enableFly)
    , m_mouseMovedCallback(EVENT_CALLBACK(CameraController::OnMouseMoved))
    , m_mouseScrolledCallback(EVENT_CALLBACK(CameraController::OnMouseScrolled))
    , m_windowResizeCallback(EVENT_CALLBACK(CameraController::OnWindowResized))
{
    Events::Subscribe<Events::MouseMovedEvent>(m_mouseMovedCallback);
    Events::Subscribe<Events::MouseScrolledEvent>(m_mouseScrolledCallback);
    Events::Subscribe<Events::WindowResizeEvent>(m_windowResizeCallback);
}

CameraController::~CameraController()
{
    Events::Unsubscribe<Events::WindowResizeEvent>(m_windowResizeCallback);
    Events::Unsubscribe<Events::MouseScrolledEvent>(m_mouseScrolledCallback);
    Events::Unsubscribe<Events::MouseMovedEvent>(m_mouseMovedCallback);
}

void CameraController::OnUpdate(float dt)
{
    ProcessInput(dt);

    if (m_updateLookAt) {
        if (!m_isFlyEnabled) {
            m_position.y = 0;
        }
        m_camera.LookAt(m_position, m_front, m_up);
        m_updateLookAt = false;
    }
}

void CameraController::ProcessInput(float dt)
{
    if (Input::IsKeyPressed(Key::W)) {
        m_position += m_front * s_cameraMovementSpeed * dt;
        m_updateLookAt = true;
    }

    if (Input::IsKeyPressed(Key::S)) {
        m_position -= m_front * s_cameraMovementSpeed * dt;
        m_updateLookAt = true;
    }

    if (Input::IsKeyPressed(Key::A)) {
        m_position -= lia::normalize(lia::cross(m_front, m_up)) * s_cameraMovementSpeed * dt;
        m_updateLookAt = true;
    }

    if (Input::IsKeyPressed(Key::D)) {
        m_position += lia::normalize(lia::cross(m_front, m_up)) * s_cameraMovementSpeed * dt;
        m_updateLookAt = true;
    }
}

void CameraController::OnMouseMoved(const Events::MouseMovedEvent& e)
{
    const float xPos = e.MouseX;
    const float yPos = e.MouseY;

    if (!m_mouseLastPosInited) {
        m_mouseLastPos.x = xPos;
        m_mouseLastPos.y = yPos;
        m_mouseLastPosInited = true;
    }

    float xoffset = xPos - m_mouseLastPos.x;
    float yoffset = m_mouseLastPos.y - yPos;

    m_mouseLastPos.x = xPos;
    m_mouseLastPos.y = yPos;

    float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    m_yaw += xoffset;
    m_pitch += yoffset;

    if (m_pitch > 89.0f)
        m_pitch = 89.0f;
    if (m_pitch < -89.0f)
        m_pitch = -89.0f;

    lia::vec3 direction;
    direction.x = cos(lia::radians(m_yaw)) * cos(lia::radians(m_pitch));
    direction.y = sin(lia::radians(m_pitch));
    direction.z = sin(lia::radians(m_yaw)) * cos(lia::radians(m_pitch));
    m_front = lia::normalize(direction);

    m_updateLookAt = true;
}

void CameraController::OnMouseScrolled(const Events::MouseScrolledEvent& e)
{
    m_fov -= e.YOffset;

    if (m_fov < 10.0f)
        m_fov = 10.0f;
    if (m_fov > 45.0f)
        m_fov = 45.0f;

    m_camera.SetProjection(lia::radians(m_fov), m_aspectRatio, m_near, m_far);
}

void CameraController::OnWindowResized(const Events::WindowResizeEvent& e)
{
    m_aspectRatio = static_cast<float>(e.Width) / static_cast<float>(e.Height);
    m_camera.SetProjection(lia::radians(m_fov), m_aspectRatio, m_near, m_far);
}

} // namespace Elven
