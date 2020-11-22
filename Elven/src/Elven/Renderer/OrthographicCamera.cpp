#include "elpch.h"
#include "OrthographicCamera.h"

namespace Elven
{
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : m_ProjectionMatrix(gdm::orthographic(left, right, bottom, top, -1.0f, 1.0f))
        , m_ViewMatrix(), m_Position(0.0f), m_Rotation(0.0f)
    {
        m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
    {
        m_ProjectionMatrix = gdm::orthographic(left, right, bottom, top, -1.0f, 1.0f);
        m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;
    }

    void OrthographicCamera::RecalculateViewMatrix()
    {
        gdm::mat4 transform = gdm::translate(gdm::mat4(), m_Position)
            * gdm::rotate(gdm::mat4(), gdm::radians(m_Rotation), gdm::vec3(0, 0, 1));

        m_ViewMatrix = gdm::inverse(transform);
        m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;
    }
}
