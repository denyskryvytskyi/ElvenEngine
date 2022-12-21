#pragma once

#include "EventHandler.h"

namespace Elven::events {

using EventId = std::uint32_t;
using HandlerId = std::uint64_t;

class EventManager {
public:
    void Shutdown();

    void Subscribe(EventId eventId, UniquePtr<IEventHandlerWrapper>&& handler, HandlerId handlerId);
    void Unsubscribe(EventId eventId, const std::string& handlerName, HandlerId handlerId);
    void TriggerEvent(const Event& event_, HandlerId handlerId);
    void QueueEvent(UniquePtr<Event>&& event_, HandlerId handlerId);
    void DispatchEvents();

private:
    std::vector<std::pair<UniquePtr<Event>, HandlerId>> m_eventsQueue;
    std::unordered_map<EventId, std::vector<UniquePtr<IEventHandlerWrapper>>> m_subscribers;
    std::unordered_map<EventId, std::unordered_map<HandlerId, std::vector<UniquePtr<IEventHandlerWrapper>>>> m_subscribersByHandlerId;
};

extern EventManager gEventManager;

template<typename EventType>
inline void Subscribe(const EventHandler<EventType>& callback, HandlerId handlerId = 0)
{
    UniquePtr<IEventHandlerWrapper> handler = MakeUniquePtr<EventHandlerWrapper<EventType>>(callback);
    gEventManager.Subscribe(EventType::GetStaticEventType(), std::move(handler), handlerId);
}

template<typename EventType>
inline void Unsubscribe(const EventHandler<EventType>& callback, HandlerId handlerId = 0)
{
    const std::string handlerName = callback.target_type().name();
    gEventManager.Unsubscribe(EventType::GetStaticEventType(), handlerName, handlerId);
}

inline void TriggerEvent(const Event& triggeredEvent, HandlerId handlerId = 0)
{
    gEventManager.TriggerEvent(triggeredEvent, handlerId);
}

inline void QueueEvent(UniquePtr<Event>&& queuedEvent, HandlerId handlerId = 0)
{
    gEventManager.QueueEvent(std::forward<UniquePtr<Event>>(queuedEvent), handlerId);
}

} // namespace Elven::events
