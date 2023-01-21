#pragma once

#include "Core/MouseCodes.h"
#include "Events/Event.h"

namespace elv::events {

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
        return std::format("MouseMovedEvent: {}, {}", mouseX, mouseY);
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
        return std::format("MouseScrolledEvent: {}, {}", xOffset, yOffset);
    }

public:
    float xOffset { 0.0f };
    float yOffset { 0.0f };
};

class MouseButtonPressedEvent : public Event {
public:
    EVENT_TYPE("MouseButtonPressedEvent")

    MouseButtonPressedEvent(int button)
        : button(button)
    {
    }

    std::string ToString() const override
    {
        return std::format("MouseButtonPressedEvent: {}", button);
    }

public:
    MouseCode button { 0 };
};

class MouseButtonReleasedEvent : public Event {
public:
    EVENT_TYPE("MouseButtonReleasedEvent")

    MouseButtonReleasedEvent(int button)
        : button(button)
    {
    }

    std::string ToString() const override
    {
        return std::format("MouseButtonReleasedEvent: {}", button);
    }

public:
    MouseCode button { 0 };
};

} // namespace elv::events
