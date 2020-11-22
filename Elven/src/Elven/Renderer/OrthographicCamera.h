#pragma once

#include <gdm/gdm.h>

namespace Elven
{
    class OrthographicCamera
    {
    public:
        OrthographicCamera(float left, float right, float top, float bottom);

        void SetProjection(float left, float right, float bottom, float top);

        void SetPosition(const gdm::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
        const gdm::vec3& GetPosition() const { return m_Position; }

        void SetRotation(const float& rotationAngle) { m_Rotation = rotationAngle; RecalculateViewMatrix(); }
        float GetRotation() const { return m_Rotation; }

        const gdm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const gdm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const gdm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

    private:
        void RecalculateViewMatrix();

    private:
        gdm::mat4 m_ProjectionMatrix;
        gdm::mat4 m_ViewMatrix;
        gdm::mat4 m_ViewProjectionMatrix;

        gdm::vec3 m_Position;

        // rotation around z-axis
        float m_Rotation;
    };
}