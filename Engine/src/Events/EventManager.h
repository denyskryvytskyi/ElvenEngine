#pragma once

#include "EventHandler.h"

namespace elv::events {

using EventType = std::uint32_t;
using EventId = std::uint64_t;

class EventManager {
public:
    EventManager() = default;
    EventManager(const EventManager&) = delete;
    const EventManager& operator=(const EventManager&) = delete;

    void Shutdown();

    void Subscribe(EventType eventType, UniquePtr<IEventHandlerWrapper>&& handler, EventId eventId);
    void Unsubscribe(EventType eventType, const std::string& handlerName, EventId eventId);
    void TriggerEvent(const Event& event_, EventId eventId);
    void QueueEvent(UniquePtr<Event>&& event_, EventId eventId);
    void DispatchEvents();

private:
    std::vector<std::pair<UniquePtr<Event>, EventId>> m_eventsQueue;
    std::unordered_map<EventType, std::vector<UniquePtr<IEventHandlerWrapper>>> m_subscribers;
    std::unordered_map<EventType, std::unordered_map<EventId, std::vector<UniquePtr<IEventHandlerWrapper>>>> m_subscribersByEventId;
};

extern EventManager gEventManager;

template<typename EventType>
inline void Subscribe(const EventHandler<EventType>& callback, EventId eventId = 0, const bool unsubscribeOnSuccess = false)
{
    UniquePtr<IEventHandlerWrapper> handler = MakeUniquePtr<EventHandlerWrapper<EventType>>(callback, unsubscribeOnSuccess);
    gEventManager.Subscribe(EventType::GetStaticEventType(), std::move(handler), eventId);
}

template<typename EventType>
inline void Unsubscribe(const EventHandler<EventType>& callback, EventId eventId = 0)
{
    const std::string handlerName = callback.target_type().name();
    gEventManager.Unsubscribe(EventType::GetStaticEventType(), handlerName, eventId);
}

inline void TriggerEvent(const Event& triggeredEvent, EventId eventId = 0)
{
    gEventManager.TriggerEvent(triggeredEvent, eventId);
}

inline void QueueEvent(UniquePtr<Event>&& queuedEvent, EventId eventId = 0)
{
    gEventManager.QueueEvent(std::forward<UniquePtr<Event>>(queuedEvent), eventId);
}

} // namespace elv::events
