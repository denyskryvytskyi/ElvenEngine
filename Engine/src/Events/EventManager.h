#pragma once

#include "EventHandler.h"

namespace Elven::events {

class EventManager {
public:
    void Shutdown();

    void Subscribe(std::uint32_t eventId, UniquePtr<IEventHandlerWrapper>&& handler);
    void Unsubscribe(std::uint32_t eventId, const std::string& handlerName);
    void TriggerEvent(const Event& event);
    void QueueEvent(UniquePtr<Event>&& event);
    void DispatchEvents();

private:
    std::vector<UniquePtr<Event>> m_eventsQueue;
    std::unordered_map<std::uint32_t, std::vector<UniquePtr<IEventHandlerWrapper>>> m_subscribers;
};

extern EventManager gEventManager;

template<typename EventType>
inline void Subscribe(const EventHandler<EventType>& callback)
{
    UniquePtr<IEventHandlerWrapper> handler = MakeUniquePtr<EventHandlerWrapper<EventType>>(callback);
    gEventManager.Subscribe(EventType::GetStaticEventType(), std::move(handler));
}

template<typename EventType>
inline void Unsubscribe(const EventHandler<EventType>& callback)
{
    const std::string handlerName = callback.target_type().name();
    gEventManager.Unsubscribe(EventType::GetStaticEventType(), handlerName);
}

inline void TriggerEvent(const Event& triggeredEvent)
{
    gEventManager.TriggerEvent(triggeredEvent);
}

inline void QueueEvent(UniquePtr<Event>&& queuedEvent)
{
    gEventManager.QueueEvent(std::forward<UniquePtr<Event>>(queuedEvent));
}

} // namespace Elven::events
