#pragma once

#include "Events/Event.h"

namespace Elven
{
    namespace Events
    {
        class WindowResizeEvent : public Event
        {
        public:
            EVENT_TYPE("{7CCF9526-19A3-431E-B9CB-B6AA7C775469}")

            WindowResizeEvent(unsigned int width, unsigned int height) :
            m_width(width),
            m_height(height) {}

            unsigned int GetWidth() const { return m_width; }
            unsigned int GetHeight() const { return m_height; }

            std::string ToString() const override
            {
                std::stringstream ss;
                ss << "WindowResizeEvent: " << m_width << ", " << m_height;
                return ss.str();
            }

        private:
            unsigned int m_width { 0 };
            unsigned int m_height { 0 };
        };

        class WindowCloseEvent : public Event
        {
        public:
            EVENT_TYPE("{1FF1FDDE-2983-4F3E-88D6-16D03D6A04A4}")
        };
    }
}