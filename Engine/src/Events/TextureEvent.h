#pragma once
#include "Events/Event.h"

namespace elv::events {

class TextureLoadedEvent : public Event {
public:
    TextureLoadedEvent(std::string_view texture_name)
        : textureName(texture_name) {};

    EVENT_TYPE("TextureLoadedEvent")

    std::string ToString() const override
    {
        return fmt::format("TextureLoadedEvent: {}", textureName);
    }

public:
    std::string_view textureName;
};

} // namespace elv::events
