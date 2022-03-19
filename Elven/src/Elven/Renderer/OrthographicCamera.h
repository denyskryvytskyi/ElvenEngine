#pragma once

#include <lia/lia.h>

namespace Elven
{
    class OrthographicCamera
    {
    public:
        OrthographicCamera(float left, float right, float top, float bottom);

        void SetProjection(float left, float right, float bottom, float top);

        void SetPosition(const lia::vec3& position) { m_Position = position; RecalculateViewMatrix(); }
        const lia::vec3& GetPosition() const { return m_Position; }

        void SetRotation(const float& rotationAngle) { m_Rotation = rotationAngle; RecalculateViewMatrix(); }
        float GetRotation() const { return m_Rotation; }

        const lia::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
        const lia::mat4& GetViewMatrix() const { return m_ViewMatrix; }
        const lia::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }

    private:
        void RecalculateViewMatrix();

    private:
        lia::mat4 m_ProjectionMatrix;
        lia::mat4 m_ViewMatrix;
        lia::mat4 m_ViewProjectionMatrix;

        lia::vec3 m_Position;

        // rotation around z-axis
        float m_Rotation;
    };
}