#pragma once

#include "Events/Event.h"

namespace Elven {
namespace Events {
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
    EVENT_TYPE("{FAAB99F1-7E9B-4533-9DE6-A84B49DD3230}")

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
    EVENT_TYPE("{59165F38-446B-47E1-994D-E67BEC9832BD}")

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
    EVENT_TYPE("{5CAFF4C9-2EF6-4A0B-9664-850CA2173AF7}")

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
} // namespace Events
} // namespace Elven
