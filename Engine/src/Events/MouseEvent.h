#pragma once

#include "Events/Event.h"

namespace Elven
{
    namespace Events
    {
        class MouseMovedEvent : public Event
        {
        public:
            EVENT_TYPE("{F40458FE-A406-4ECF-9EB8-D603716E4E4E}")

            MouseMovedEvent(float x, float y) :
                m_mouseX(x),
                m_mouseY(y)
            {}

            float GetX() const { return m_mouseX; }
            float GetY() const { return m_mouseY; }

            std::string ToString() const override
            {
                std::stringstream ss;
                ss << "MouseMovedEvent: " << m_mouseX << ", " << m_mouseY;
                return ss.str();
            }

        private:
            float m_mouseX { 0.0f };
            float m_mouseY { 0.0f };
        };

        class MouseScrolledEvent : public Event
        {
        public:
            EVENT_TYPE("{961D7FE1-F601-47C4-BB5F-0D93F601DD2D}")

                MouseScrolledEvent(float xOffset, float yOffset) :
                m_xOffset(xOffset),
                m_yOffset(yOffset)
            {}

            float GetXOffset() const { return m_xOffset; }
            float GetYOffset() const { return m_yOffset; }

            std::string ToString() const override
            {
                std::stringstream ss;
                ss << "MouseScrolledEvent: " << m_xOffset << ", " << m_yOffset;
                return ss.str();
            }

        private:
            float m_xOffset = 0.0f;
            float m_yOffset = 0.0f;
        };

        class MouseButtonEvent : public Event
        {
        public:
            EVENT_TYPE("{BC44A1F0-989F-4013-8516-005BA864CB12}")

            int GetMouseButton() const { return m_button; }

        protected:
            MouseButtonEvent(int button) :
                m_button(button)
            {}

        protected:
            int m_button { 0 };
        };

        class MouseButtonPressedEvent : public MouseButtonEvent
        {
        public:
            EVENT_TYPE("{BF946F16-6077-4550-A579-CCF199E83C45}")

            MouseButtonPressedEvent(int button) :
                MouseButtonEvent(button)
            {}

            std::string ToString() const override
            {
                std::stringstream ss;
                ss << "MouseButtonPressedEvent: " << m_button;
                return ss.str();
            }
        };

        class MouseButtonReleasedEvent : public MouseButtonEvent
        {
        public:
            EVENT_TYPE("{FDD0D4DB-E81F-450F-9472-81A794A5F92E}")

            MouseButtonReleasedEvent(int button) :
                MouseButtonEvent(button)
            {}

            std::string ToString() const override
            {
                std::stringstream ss;
                ss << "MouseButtonReleasedEvent: " << m_button;
                return ss.str();
            }
        };
    }
}