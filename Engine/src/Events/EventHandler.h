#pragma once

#include "Events/Event.h"

#include <functional>

namespace Elven::events {

template<typename EventType>
using EventHandler = std::function<void(const EventType& e)>;

class EventHandlerWrapperInterface {
public:
    virtual ~EventHandlerWrapperInterface() = default;

    void Exec(const Event& e)
    {
        Call(e);
    }

    virtual std::string GetType() const = 0;

private:
    virtual void Call(const Event& e) = 0;
};

template<typename EventType>
class EventHandlerWrapper : public EventHandlerWrapperInterface {
public:
    explicit EventHandlerWrapper(const EventHandler<EventType>& handler)
        : m_handler(handler)
        , m_handlerType(m_handler.target_type().name()) {};

private:
    void Call(const Event& e) override
    {
        if (e.GetEventType() == EventType::GetStaticEventType()) {
            m_handler(static_cast<const EventType&>(e));
        }
    }

    std::string GetType() const override { return m_handlerType; }

    EventHandler<EventType> m_handler;
    const std::string m_handlerType;
};
} // namespace Elven::events
