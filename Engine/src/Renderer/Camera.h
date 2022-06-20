#pragma once

#include <lia/lia.h>

namespace Elven {

class Camera {
public:
    Camera(float fov, float aspect, float near_, float far_);
    Camera(float left, float right, float bottom, float top, float near_, float far_);

    void SetProjection(float fov, float aspect, float near_, float far_);
    void SetProjection(float left, float right, float bottom, float top, float near_, float far_);

    void SetPosition(const lia::vec3& position)
    {
        m_position = position;
        RecalculateViewMatrix();
    }
    const lia::vec3& GetPosition() const { return m_position; }

    /**
     * @param rotation: vector where particular component is rotation angle in degrees around coresponding axis
     */
    void SetRotation(const lia::vec3& rotation)
    {
        m_rotation = rotation;
        RecalculateViewMatrix();
    }
    lia::vec3 GetRotation() const { return m_rotation; }

    const lia::mat4& GetProjectionMatrix() const { return m_projectionMatrix; }
    const lia::mat4& GetViewMatrix() const { return m_viewMatrix; }
    const lia::mat4& GetViewProjectionMatrix() const { return m_viewProjectionMatrix; }

    void LookAt(const lia::vec3 pos, const lia::vec3 front, const lia::vec3 up);

private:
    void RecalculateViewMatrix();

private:
    bool m_isPerspective { false };

    lia::mat4 m_viewMatrix { 1.0f };
    lia::mat4 m_projectionMatrix;
    lia::mat4 m_viewProjectionMatrix;

    lia::vec3 m_position;
    lia::vec3 m_rotation;
};

} // namespace Elven
