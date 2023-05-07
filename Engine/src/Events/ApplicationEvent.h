#pragma once

#include "Events/Event.h"

#include <fmt/format.h>

namespace elv::events {

class WindowResizeEvent : public Event {
public:
    EVENT_TYPE("WindowResizeEvent")

    WindowResizeEvent(unsigned int width_, unsigned int height_)
        : width(width_)
        , height(height_)
    {
    }

    std::string ToString() const override
    {
        return fmt::format("WindowResizeEvent: {}, {}", width, height);
    }

public:
    unsigned int width { 0 };
    unsigned int height { 0 };
};

class WindowCloseEvent : public Event {
public:
    EVENT_TYPE("WindowCloseEvent")
};

} // namespace elv::events
