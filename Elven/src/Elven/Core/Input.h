#pragma once

#include "Elven/Core/Core.h"

#include "Elven/Core/KeyCodes.h"
#include "Elven/Core/MouseCodes.h"

namespace Elven
{
    class Input
    {
    public:
        static bool IsKeyPressed(KeyCode key);

        static bool IsMousePressed(MouseCode button);
        static std::pair<float, float> GetMousePosition();
        static float GetMouseX();
        static float GetMouseY();
    };


}