#pragma once

#include "Camera.h"

namespace Elven
{
    class CameraController
    {
    public:
        CameraController(float fov, float aspectRatio, float near_, float far_);

        void OnUpdate(float dt);

        Camera& GetCamera() { return m_camera; }
        const Camera& GetCamera() const { return m_camera; }

    private:
        Camera m_camera;
    };
}