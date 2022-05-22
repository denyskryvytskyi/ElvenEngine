#include "CameraController.h"

namespace Elven
{
    CameraController::CameraController(float fov, float aspectRatio, float near_, float far_)
        : m_camera(fov, aspectRatio, near_, far_)
    {
    }

    void CameraController::OnUpdate(float dt)
    {
    }
}