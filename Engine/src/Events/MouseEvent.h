#pragma once

#include "Events/Event.h"

namespace Elven::Events {

class MouseMovedEvent : public Event {
public:
    EVENT_TYPE("MouseMovedEvent")

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
    EVENT_TYPE("MouseScrolledEvent")

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
    EVENT_TYPE("MouseButtonEvent")

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
    EVENT_TYPE("MouseButtonPressedEvent")

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
    EVENT_TYPE("MouseButtonReleasedEvent")

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

} // namespace Elven::Events
