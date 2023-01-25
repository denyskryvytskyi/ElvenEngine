#pragma once

#include "Core/KeyCodes.h"
#include "Core/MouseCodes.h"

namespace elv {

class Input {
public:
    static bool IsKeyPressed(Keycode key);

    static bool IsMousePressed(MouseCode button);
    static std::pair<float, float> GetMousePosition();
    static float GetMouseX();
    static float GetMouseY();
};

} // namespace elv
