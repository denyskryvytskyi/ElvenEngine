
#include "OrthographicCamera.h"

namespace Elven
{
    OrthographicCamera::OrthographicCamera(float left, float right, float bottom, float top)
        : m_ProjectionMatrix(lia::orthographic(left, right, bottom, top, -1.0f, 1.0f))
        , m_ViewMatrix(1.0f), m_Position(0.0f), m_Rotation(0.0f)
    {
        m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;
    }

    void OrthographicCamera::SetProjection(float left, float right, float bottom, float top)
    {
        m_ProjectionMatrix = lia::orthographic(left, right, bottom, top, -1.0f, 1.0f);
        m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;
    }

    void OrthographicCamera::RecalculateViewMatrix()
    {
        lia::mat4 transform = lia::rotate(lia::mat4(1.0f), lia::radians(m_Rotation), lia::vec3(0, 0, 1));
        transform = lia::translate(transform, m_Position);

        m_ViewMatrix = lia::inverse(transform);
        m_ViewProjectionMatrix = m_ViewMatrix * m_ProjectionMatrix;
    }
}
