#include "Events/EventManager.h"

namespace Elven::events {

EventManager gEventManager;

void EventManager::Shutdown()
{
    m_subscribers.clear();
}

void EventManager::Subscribe(std::uint32_t eventId, UniquePtr<IEventHandlerWrapper>&& handler)
{
    auto subscribers = m_subscribers.find(eventId);
    if (subscribers != m_subscribers.end()) {
        auto& handlers = subscribers->second;
        for (auto& it : handlers) {
            if (it->GetType() == handler->GetType()) {
                EL_ASSERT(false, "Attempting to double-register callback");
                return;
            }
        }
        handlers.emplace_back(std::move(handler));
    } else {
        m_subscribers[eventId].emplace_back(std::move(handler));
    }
}

void EventManager::Unsubscribe(std::uint32_t eventId, const std::string& handlerName)
{
    auto& handlers = m_subscribers[eventId];
    for (auto& it = handlers.begin(); it != handlers.end(); ++it) {
        if (it->get()->GetType() == handlerName) {
            it = handlers.erase(it);
            return;
        }
    }
}

void EventManager::TriggerEvent(const Event& event_)
{
    for (auto& handler : m_subscribers[event_.GetEventType()]) {
        handler->Exec(event_);
    }
}

void EventManager::QueueEvent(UniquePtr<Event>&& event_)
{
    m_eventsQueue.emplace_back(std::move(event_));
}

void EventManager::DispatchEvents()
{
    for (auto& eventIt = m_eventsQueue.begin(); eventIt != m_eventsQueue.end();) {
        if (!eventIt->get()->isHandled) {
            TriggerEvent(*eventIt->get());
            eventIt = m_eventsQueue.erase(eventIt);
        } else {
            ++eventIt;
        }
    }
}

} // namespace Elven::events
