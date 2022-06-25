#pragma once

#include <functional>

#include "Events/Event.h"

#define EVENT_CALLBACK(fn) [this](auto&& event_) { fn(std::forward<decltype(event_)>(event_)); }

namespace Elven::Events {

class EventCallbackWrapper {
public:
    void exec(const Event& e)
    {
        call(e);
    }

    virtual const char* getType() const = 0;

private:
    virtual void call(const Event& e) = 0;
};

template<typename EventType>
using EventFunctionHandler = std::function<void(const EventType& e)>;

template<typename EventType>
class EventCallbackWrapperT : public EventCallbackWrapper {
public:
    explicit EventCallbackWrapperT(const EventFunctionHandler<EventType>& callback)
        : functionHandler(callback)
        , functionType(functionHandler.target_type().name()) {};

private:
    void call(const Event& e) override
    {
        if (e.GetEventType() == EventType::GetStaticEventType()) {
            functionHandler(static_cast<const EventType&>(e));
        }
    }

    const char* getType() const override { return functionType; }

    EventFunctionHandler<EventType> functionHandler;
    const char* functionType;
};

class EventManager {
public:
    void Shutdown();

    void Subscribe(std::uint32_t eventId, UniquePtr<EventCallbackWrapper>& handler);
    void Unsubscribe(std::uint32_t eventId, const char* handlerName);
    void TriggerEvent(const Event& event);
    void QueueEvent(Event* event);
    void DispatchEvents();

private:
    std::vector<Event*> m_eventsQueue;
    std::unordered_map<std::uint32_t, std::vector<UniquePtr<EventCallbackWrapper>>> m_subscribers;
};

extern EventManager gEventManager;

template<typename EventType>
static void Subscribe(const EventFunctionHandler<EventType>& callback)
{
    UniquePtr<EventCallbackWrapper> handler = MakeUniquePtr<EventCallbackWrapperT<EventType>>(callback);

    gEventManager.Subscribe(EventType::GetStaticEventType(), handler);
}

template<typename EventType>
static void Unsubscribe(const EventFunctionHandler<EventType>& callback)
{
    const char* handlerName = callback.target_type().name();
    gEventManager.Unsubscribe(EventType::GetStaticEventType(), handlerName);
}

static void TriggerEvent(const Event& triggeredEvent)
{
    gEventManager.TriggerEvent(triggeredEvent);
}

static void QueueEvent(Event* queuedEvent)
{
    gEventManager.QueueEvent(queuedEvent);
}

} // namespace Elven::Events
