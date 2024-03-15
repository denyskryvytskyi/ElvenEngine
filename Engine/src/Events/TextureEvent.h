#pragma once
#include "Events/Event.h"

namespace elv::events {

class TextureLoadedEvent : public Event {
public:
    TextureLoadedEvent(const std::string& texture_name)
        : textureName(texture_name) {};

    EVENT_TYPE("TextureLoadedEvent")

    std::string ToString() const override
    {
        return fmt::format("TextureLoadedEvent: {}", textureName);
    }

public:
    std::string textureName;
};

} // namespace elv::events
