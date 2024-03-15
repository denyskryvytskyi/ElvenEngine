#pragma once

#include "Events/Event.h"

#include <functional>

namespace elv::events {

template<typename EventType>
using EventHandler = std::function<void(const EventType& e)>;

class IEventHandlerWrapper {
public:
    virtual ~IEventHandlerWrapper() = default;

    void Exec(const Event& e)
    {
        Call(e);
    }

    virtual std::string GetType() const = 0;
    virtual bool IsDestroyOnSuccess() const = 0;

private:
    virtual void Call(const Event& e) = 0;
};

template<typename EventType>
class EventHandlerWrapper : public IEventHandlerWrapper {
public:
    explicit EventHandlerWrapper(const EventHandler<EventType>& handler, const bool destroyOnSuccess = false)
        : m_handler(handler)
        , m_handlerType(m_handler.target_type().name())
        , m_destroyOnSuccess(destroyOnSuccess)
    { }

private:
    void Call(const Event& e) override
    {
        if (e.GetEventType() == EventType::GetStaticEventType()) {
            m_handler(static_cast<const EventType&>(e));
        }
    }

    std::string GetType() const override { return m_handlerType; }
    bool IsDestroyOnSuccess() const { return m_destroyOnSuccess; }

    EventHandler<EventType> m_handler;
    const std::string m_handlerType;
    bool m_destroyOnSuccess { false };
};
} // namespace elv::events
