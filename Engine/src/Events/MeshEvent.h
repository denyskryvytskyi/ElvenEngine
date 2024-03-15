#pragma once
#include "Events/Event.h"

namespace elv::events {

class MeshLoadedEvent : public Event {
public:
    MeshLoadedEvent(std::string_view meshName)
        : meshName(meshName) {};

    EVENT_TYPE("MeshLoadedEvent")

    std::string ToString() const override
    {
        return fmt::format("MeshLoadedEvent: {}", meshName);
    }

public:
    std::string_view meshName;
};

} // namespace elv::events
