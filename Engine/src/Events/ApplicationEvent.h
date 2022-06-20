#pragma once

#include "Events/Event.h"

namespace Elven {
    namespace Events {
        class WindowResizeEvent : public Event
        {
        public:
            EVENT_TYPE("{7CCF9526-19A3-431E-B9CB-B6AA7C775469}")

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

        class WindowCloseEvent : public Event
        {
        public:
            EVENT_TYPE("{1FF1FDDE-2983-4F3E-88D6-16D03D6A04A4}")
        };

    } // namespace Events
} // namespace Elven
