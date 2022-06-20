#pragma once

#include "Events/Event.h"

namespace Elven {
namespace Events {
class MouseMovedEvent : public Event {
public:
    EVENT_TYPE("{F40458FE-A406-4ECF-9EB8-D603716E4E4E}")

    MouseMovedEvent(float x, float y)
        : MouseX(x)
        , MouseY(y)
    {
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseMovedEvent: " << MouseX << ", " << MouseY;
        return ss.str();
    }

public:
    float MouseX { 0.0f };
    float MouseY { 0.0f };
};

class MouseScrolledEvent : public Event {
public:
    EVENT_TYPE("{961D7FE1-F601-47C4-BB5F-0D93F601DD2D}")

    MouseScrolledEvent(float xOffset, float yOffset)
        : XOffset(xOffset)
        , YOffset(yOffset)
    {
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseScrolledEvent: " << XOffset << ", " << YOffset;
        return ss.str();
    }

public:
    float XOffset { 0.0f };
    float YOffset { 0.0f };
};

class MouseButtonEvent : public Event {
public:
    EVENT_TYPE("{BC44A1F0-989F-4013-8516-005BA864CB12}")

protected:
    MouseButtonEvent(int button)
        : Button(button)
    {
    }

public:
    int Button { 0 };
};

class MouseButtonPressedEvent : public MouseButtonEvent {
public:
    EVENT_TYPE("{BF946F16-6077-4550-A579-CCF199E83C45}")

    MouseButtonPressedEvent(int button)
        : MouseButtonEvent(button)
    {
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseButtonPressedEvent: " << Button;
        return ss.str();
    }
};

class MouseButtonReleasedEvent : public MouseButtonEvent {
public:
    EVENT_TYPE("{FDD0D4DB-E81F-450F-9472-81A794A5F92E}")

    MouseButtonReleasedEvent(int button)
        : MouseButtonEvent(button)
    {
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseButtonReleasedEvent: " << Button;
        return ss.str();
    }
};
} // namespace Events
} // namespace Elven
