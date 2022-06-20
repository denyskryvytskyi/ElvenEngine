#include "Events/EventManager.h"

namespace Elven {
namespace Events {

EventManager gEventManager;

void EventManager::Shutdown()
{
    for (auto event : m_eventsQueue) {
        delete event;
    }

    for (auto& eventSubscirbersIt = m_subscribers.begin(); eventSubscirbersIt != m_subscribers.end(); ++eventSubscirbersIt) {
        auto& handlers = eventSubscirbersIt->second;
        for (auto& it = handlers.begin(); it != handlers.end();) {
            delete *it;
            it = handlers.erase(it);
        }
    }
    m_subscribers.clear();
}

void EventManager::Subscribe(const std::string eventId, EventCallbackWrapper* handler)
{
    auto subscribers = m_subscribers.find(eventId);
    if (subscribers != m_subscribers.end()) {
        auto& handlers = subscribers->second;
        for (auto it = handlers.begin(); it != handlers.end(); ++it) {
            if ((*it)->getType() == handler->getType()) {
                EL_ASSERT(false, "Attempting to double-register callback");
                return;
            }
        }
        handlers.emplace_back(handler);
    } else {
        m_subscribers[eventId].emplace_back(handler);
    }
}

void EventManager::Unsubscribe(const std::string eventId, const char* handlerName)
{
    auto& handlers = m_subscribers[eventId];
    for (auto it = handlers.begin(); it != handlers.end(); ++it) {
        if ((*it)->getType() == handlerName) {
            delete *it;
            it = handlers.erase(it);
            return;
        }
    }
}

void EventManager::TriggerEvent(Event* event)
{
    for (auto handler : m_subscribers[event->GetEventType()]) {
        handler->exec(*event);
    }
    event->Handled = true;
}

void EventManager::QueueEvent(Event* event)
{
    m_eventsQueue.emplace_back(event);
}

void EventManager::DispatchEvents()
{
    for (auto eventIt = m_eventsQueue.begin(); eventIt != m_eventsQueue.end();) {
        if (!(*eventIt)->Handled) {
            TriggerEvent(*eventIt);
            eventIt = m_eventsQueue.erase(eventIt);
        } else {
            ++eventIt;
        }
    }
}

} // namespace Events
} // namespace Elven
