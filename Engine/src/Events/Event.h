#pragma once

#include <sstream>
#include <string>

namespace Elven::Events {

class Event {
public:
    virtual const std::string GetEventType() const = 0;
    virtual std::string ToString() const { return GetEventType(); };

    bool Handled = false;
};

#define EVENT_TYPE(event_type)                                           \
    static const std::string GetStaticEventType() { return event_type; } \
    const std::string GetEventType() const override { return GetStaticEventType(); }

inline std::ostream& operator<<(std::ostream& os, const Event& e)
{
    return os << e.ToString();
}

} // namespace Elven::Events
