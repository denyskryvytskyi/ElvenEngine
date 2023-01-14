#pragma once

#include "Core/KeyCodes.h"
#include "Events/Event.h"

namespace elv::events {

class KeyEvent : public Event {
public:
    EVENT_TYPE("{498CE06D-7EE3-465F-9EA7-B6D6E8CE963D}")

protected:
    KeyEvent(KeyCode keyCode)
        : key(keyCode)
    {
    }

public:
    KeyCode key { 0 };
};

class KeyPressedEvent : public KeyEvent {
public:
    EVENT_TYPE("KeyPressedEvent")

    KeyPressedEvent(int keyCode, int repeatCount)
        : KeyEvent(keyCode)
        , repeatCount(repeatCount)
    {
    }

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "KeyPressedEvent: " << key << " (" << repeatCount << " repeats)";
        return ss.str();
    }

public:
    int repeatCount { 0 };
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
        ss << "KeyReleasedEvent: " << key;
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
        ss << "KeyTypedEvent: " << key;
        return ss.str();
    }
};

} // namespace elv::events
