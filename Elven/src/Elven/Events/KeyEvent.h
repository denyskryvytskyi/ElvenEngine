#pragma once

#include "Elven/Events/Event.h"

namespace Elven
{

    class ELVEN_API KeyEvent : public Event
    {
    public:
        int GetKeyCode() const { return m_KeyCode; }

        EVENT_CLASS_CATEGORY(EventCategory_Keybord | EventCategory_Input)

    protected:
        KeyEvent(int keyCode)
            : m_KeyCode(keyCode) {}

        int m_KeyCode;
    };

    class ELVEN_API KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(int keyCode, int repeatCount)
            : KeyEvent(keyCode),
            m_RepeatCount(repeatCount) {}

        int GetRepeatCount() const { return m_RepeatCount; }

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyPressedEvent: " << m_KeyCode << " (" << m_RepeatCount << " repeats)";
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyPressed)

    private:
        int m_RepeatCount;
    };

    class ELVEN_API KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(int keyCode)
            : KeyEvent(keyCode) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyReleasedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyReleased)
    };

    class ELVEN_API KeyTypedEvent : public KeyEvent
    {
    public:
        KeyTypedEvent(int keyCode)
            : KeyEvent(keyCode) {}

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << "KeyTypedEvent: " << m_KeyCode;
            return ss.str();
        }

        EVENT_CLASS_TYPE(KeyTyped)
    };
}