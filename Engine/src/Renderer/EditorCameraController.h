#pragma once

#include "CameraController.h"

#include "Events/MouseEvent.h"

namespace elv {

class EditorCameraController : public CameraController {
public:
    EditorCameraController(float fov, float aspectRatio, float near_, float far_, bool enableFly = true);

private:
    void ProcessInput(float dt) override;

    void OnMouseButtonPressed(const events::MouseButtonPressedEvent& e);
    void OnMouseButtonRelease(const events::MouseButtonReleasedEvent& e);

private:
    events::EventHandler<events::MouseButtonPressedEvent> m_mouseButtonPressedCallback;
    events::EventHandler<events::MouseButtonReleasedEvent> m_mouseButtonReleaseCallback;
};

} // namespace elv
