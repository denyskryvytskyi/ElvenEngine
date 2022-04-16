#pragma once

#include <string>
#include <sstream>

namespace Elven
{
    namespace Events
    {
        class Event
        {
        public:
            virtual const std::string GetEventType() const = 0;
            virtual std::string ToString() const { return GetEventType(); };

        public:
            bool Handled = false;
        };

#define EVENT_TYPE(event_type) static const std::string GetStaticEventType() { return event_type; } \
                               virtual const std::string GetEventType() const { return GetStaticEventType(); }

        inline std::ostream& operator <<(std::ostream& os, const Event& e)
        {
            return os << e.ToString();
        }
    }
}