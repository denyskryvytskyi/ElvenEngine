#pragma once

#include "Core/KeyCodes.h"
#include "Events/Event.h"

namespace elv::events {

class KeyPressedEvent : public Event {
public:
    EVENT_TYPE("KeyPressedEvent")

    KeyPressedEvent(int keyCode, int repeatCount)
        : key(keyCode)
        , repeatCount(repeatCount)
    {
    }

    std::string ToString() const override
    {
        return fmt::format("KeyPressedEvent: {} ({} repeats)", key, repeatCount);
    }

public:
    Keycode key { 0 };
    int repeatCount { 0 };
};

class KeyReleasedEvent : public Event {
public:
    EVENT_TYPE("KeyReleasedEvent")

    KeyReleasedEvent(int keyCode)
        : key(keyCode)
    {
    }

    std::string ToString() const override
    {
        return fmt::format("KeyReleasedEvent: {}", key);
    }

public:
    Keycode key { 0 };
};

class KeyTypedEvent : public Event {
public:
    EVENT_TYPE("KeyTypedEvent")

    KeyTypedEvent(int keyCode)
        : key(keyCode)
    {
    }

    std::string ToString() const override
    {
        return fmt::format("KeyTypedEvent: {}", key);
    }

public:
    Keycode key { 0 };
};

} // namespace elv::events
