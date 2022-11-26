#pragma once

#include "Core/MouseCodes.h"
#include "Events/Event.h"

namespace Elven::events {

class MouseMovedEvent : public Event {
public:
    EVENT_TYPE("MouseMovedEvent")

    MouseMovedEvent(float x, float y)
        : mouseX(x)
        , mouseY(y)
    {
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseMovedEvent: " << mouseX << ", " << mouseY;
        return ss.str();
    }

public:
    float mouseX { 0.0f };
    float mouseY { 0.0f };
};

class MouseScrolledEvent : public Event {
public:
    EVENT_TYPE("MouseScrolledEvent")

    MouseScrolledEvent(float xOffset_, float yOffset_)
        : xOffset(xOffset_)
        , yOffset(yOffset_)
    {
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "MouseScrolledEvent: " << xOffset << ", " << yOffset;
        return ss.str();
    }

public:
    float xOffset { 0.0f };
    float yOffset { 0.0f };
};

class MouseButtonEvent : public Event {
public:
    EVENT_TYPE("MouseButtonEvent")

protected:
    MouseButtonEvent(MouseCode button_)
        : button(button_)
    {
    }

public:
    MouseCode button { 0 };
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
        ss << "MouseButtonPressedEvent: " << button;
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
        ss << "MouseButtonReleasedEvent: " << button;
        return ss.str();
    }
};

} // namespace Elven::events
