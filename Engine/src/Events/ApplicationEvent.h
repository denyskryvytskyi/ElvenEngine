#pragma once

#include "Events/Event.h"

namespace Elven::Events {

class WindowResizeEvent : public Event {
public:
    EVENT_TYPE("WindowResizeEvent")

    WindowResizeEvent(unsigned int width, unsigned int height)
        : Width(width)
        , Height(height)
    {
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "WindowResizeEvent: " << Width << ", " << Height;
        return ss.str();
    }

public:
    unsigned int Width { 0 };
    unsigned int Height { 0 };
};

class WindowCloseEvent : public Event {
public:
    EVENT_TYPE("WindowCloseEvent")
};

} // namespace Elven::Events
