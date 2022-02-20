#pragma once

#include "Elven/Events/Event.h"

namespace Elven
{
    namespace Events
    {
        class KeyEvent : public Event
        {
        public:
            EVENT_ID("{498CE06D-7EE3-465F-9EA7-B6D6E8CE963D}")

                int GetKeyCode() const { return m_keyCode; }

        protected:
            KeyEvent(int keyCode) :
                m_keyCode(keyCode)
            {}

            int m_keyCode;
        };

        class KeyPressedEvent : public KeyEvent
        {
        public:
            EVENT_ID("{FAAB99F1-7E9B-4533-9DE6-A84B49DD3230}")

                KeyPressedEvent(int keyCode, int repeatCount) : KeyEvent(keyCode),
                m_repeatCount(repeatCount)
            {}

            int GetRepeatCount() const { return m_repeatCount; }

            std::string ToString() const override
            {
                std::stringstream ss;
                ss << "KeyPressedEvent: " << m_keyCode << " (" << m_repeatCount << " repeats)";
                return ss.str();
            }

        private:
            int m_repeatCount;
        };

        class KeyReleasedEvent : public KeyEvent
        {
        public:
            EVENT_ID("{59165F38-446B-47E1-994D-E67BEC9832BD}")

                KeyReleasedEvent(int keyCode) : KeyEvent(keyCode) {}

            std::string ToString() const override
            {
                std::stringstream ss;
                ss << "KeyReleasedEvent: " << m_keyCode;
                return ss.str();
            }
        };

        class KeyTypedEvent : public KeyEvent
        {
        public:
            EVENT_ID("{5CAFF4C9-2EF6-4A0B-9664-850CA2173AF7}")

                KeyTypedEvent(int keyCode) : KeyEvent(keyCode) {}

            std::string ToString() const override
            {
                std::stringstream ss;
                ss << "KeyTypedEvent: " << m_keyCode;
                return ss.str();
            }
        };
    }
}