#include "Camera.h"

namespace Elven {
Camera::Camera(float fov, float aspect, float near_, float far_)
    : m_isPerspective(true)
    , m_projectionMatrix(lia::perspective(fov, aspect, near_, far_))
{
    m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
}

Camera::Camera(float left, float right, float bottom, float top, float near_, float far_)
    : m_projectionMatrix(lia::orthographic(left, right, bottom, top, near_, far_))
{
    m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
}

void Camera::SetProjection(float fov, float aspect, float near_, float far_)
{
    if (m_isPerspective) {
        m_projectionMatrix = lia::perspective(fov, aspect, near_, far_);
        m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
    } else {
        EL_CORE_WARN("Wrong projection type! Use ortho arguments instead.");
    }
}

void Camera::SetProjection(float left, float right, float bottom, float top, float near_, float far_)
{
    if (!m_isPerspective) {
        m_projectionMatrix = lia::orthographic(left, right, bottom, top, near_, far_);
        m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
    } else {
        EL_CORE_WARN("Wrong projection type! Use perspective arguments instead.");
    }
}

void Camera::LookAt(const lia::vec3 pos, const lia::vec3 front, const lia::vec3 up)
{
    m_viewMatrix = lia::lookAt(pos, pos + front, up);

    m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
}

void Camera::RecalculateViewMatrix()
{
    lia::mat4 transform = lia::rotate(lia::mat4(1.0f), lia::radians(m_rotation.x), lia::vec3(1, 0, 0));
    transform = lia::rotate(transform, lia::radians(m_rotation.y), lia::vec3(0, 1, 0));
    transform = lia::rotate(transform, lia::radians(m_rotation.z), lia::vec3(0, 0, 1));
    transform = lia::translate(transform, m_position);

    m_viewMatrix = lia::inverse(transform);
    m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;
}
} // namespace Elven
