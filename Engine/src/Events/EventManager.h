#pragma once

#include <functional>

#include "Events/Event.h"

#define EVENT_CALLBACK(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Elven {
namespace Events {
class EventCallbackWrapper {
public:
    void exec(Event& e)
    {
        call(e);
    }

    virtual const char* getType() const = 0;

private:
    virtual void call(Event& e) = 0;
};

template<typename EventType>
using EventFunctionHandler = std::function<void(EventType& e)>;

template<typename EventType>
class EventCallbackWrapperT : public EventCallbackWrapper {
public:
    EventCallbackWrapperT(EventFunctionHandler<EventType> callback)
        : functionHandler(callback)
        , functionType(functionHandler.target_type().name()) {};

private:
    virtual void call(Event& e) override
    {
        if (e.GetEventType() == EventType::GetStaticEventType()) {
            functionHandler(static_cast<EventType&>(e));
        }
    }

    virtual const char* getType() const override { return functionType; }

private:
    EventFunctionHandler<EventType> functionHandler;
    const char* functionType;
};

class EventManager {
public:
    void Shutdown();

    void Subscribe(const std::string eventId, EventCallbackWrapper* handler);
    void Unsubscribe(const std::string eventId, const char* handlerName);
    void TriggerEvent(Event* event);
    void QueueEvent(Event* event);
    void DispatchEvents();

private:
    std::vector<Event*> m_eventsQueue;
    std::unordered_map<std::string, std::vector<EventCallbackWrapper*>> m_subscribers;
};

extern EventManager gEventManager;

template<typename EventType>
static void Subscribe(const EventFunctionHandler<EventType>& callback)
{
    EventCallbackWrapper* handler = new EventCallbackWrapperT<EventType>(callback);

    gEventManager.Subscribe(EventType::GetStaticEventType(), handler);
}

template<typename EventType>
static void Unsubscribe(const EventFunctionHandler<EventType>& callback)
{
    const char* handlerName = callback.target_type().name();
    gEventManager.Unsubscribe(EventType::GetStaticEventType(), handlerName);
}

static void TriggerEvent(Event* triggeredEvent)
{
    gEventManager.TriggerEvent(triggeredEvent);
}

static void QueueEvent(Event* queuedEvent)
{
    gEventManager.QueueEvent(queuedEvent);
}
} // namespace Events
} // namespace Elven
