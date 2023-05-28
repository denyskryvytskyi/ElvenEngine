#pragma once
#include "Events/Event.h"
#include "Scene/Entity.h"

namespace elv::events {

class CollisionEnter2dEvent : public Event {
public:
    CollisionEnter2dEvent(ecs::Entity entity1, ecs::Entity entity2)
        : entity1(entity1)
        , entity2(entity2) {};

    EVENT_TYPE("CollisionEnter2dEvent")

    std::string ToString() const override
    {
        return fmt::format("CollisionEnter2dEvent: {} {}", entity1, entity2);
    }

public:
    ecs::Entity entity1;
    ecs::Entity entity2;
};

} // namespace elv::events
