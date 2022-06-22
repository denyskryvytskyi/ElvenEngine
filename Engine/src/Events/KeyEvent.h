#pragma once

#include "Events/Event.h"

namespace Elven::Events {

class KeyEvent : public Event {
public:
    EVENT_TYPE("{498CE06D-7EE3-465F-9EA7-B6D6E8CE963D}")

protected:
    KeyEvent(int keyCode)
        : KeyCode(keyCode)
    {
    }

public:
    int KeyCode { 0 };
};

class KeyPressedEvent : public KeyEvent {
public:
    EVENT_TYPE("KeyPressedEvent")

    KeyPressedEvent(int keyCode, int repeatCount)
        : KeyEvent(keyCode)
        , RepeatCount(repeatCount)
    {
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << KeyCode << " (" << RepeatCount << " repeats)";
        return ss.str();
    }

public:
    int RepeatCount { 0 };
};

class KeyReleasedEvent : public KeyEvent {
public:
    EVENT_TYPE("KeyReleasedEvent")

    KeyReleasedEvent(int keyCode)
        : KeyEvent(keyCode)
    {
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyReleasedEvent: " << KeyCode;
        return ss.str();
    }
};

class KeyTypedEvent : public KeyEvent {
public:
    EVENT_TYPE("KeyTypedEvent")

    KeyTypedEvent(int keyCode)
        : KeyEvent(keyCode)
    {
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyTypedEvent: " << KeyCode;
        return ss.str();
    }
};

} // namespace Elven::Events
