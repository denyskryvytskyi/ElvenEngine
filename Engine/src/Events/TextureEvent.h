#pragma once
#include "Events/Event.h"

namespace Elven::events {

class TextureLoadedEvent : public Event {
public:
    TextureLoadedEvent(const std::string& textureName_)
        : textureName(textureName_) {};

    EVENT_TYPE("TextureLoadedEvent")

    std::string ToString() const override
    {
        std::stringstream ss;
        ss << "TextureLoadedEvent: " << textureName << ", " << textureName;
        return ss.str();
    }

public:
    std::string textureName;
};

} // namespace Elven::events
