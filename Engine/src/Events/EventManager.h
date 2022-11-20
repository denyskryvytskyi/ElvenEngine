#pragma once

#include "Events/Event.h"

#include <functional>

namespace Elven::Events {

class EventCallbackBase {
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
class EventCallback : public EventCallbackBase {
public:
    explicit EventCallback(const EventFunctionHandler<EventType>& callback)
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

    void Subscribe(std::uint32_t eventId, UniquePtr<EventCallbackBase>& handler);
    void Unsubscribe(std::uint32_t eventId, const char* handlerName);
    void TriggerEvent(const Event& event);
    void QueueEvent(UniquePtr<Event>&& event);
    void DispatchEvents();

private:
    std::vector<UniquePtr<Event>> m_eventsQueue;
    std::unordered_map<std::uint32_t, std::vector<UniquePtr<EventCallbackBase>>> m_subscribers;
};

extern EventManager gEventManager;

template<typename EventType>
static void Subscribe(const EventFunctionHandler<EventType>& callback)
{
    UniquePtr<EventCallbackBase> handler = MakeUniquePtr<EventCallback<EventType>>(callback);

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

static void QueueEvent(UniquePtr<Event>&& queuedEvent)
{
    gEventManager.QueueEvent(std::forward<UniquePtr<Event>>(queuedEvent));
}

} // namespace Elven::Events
