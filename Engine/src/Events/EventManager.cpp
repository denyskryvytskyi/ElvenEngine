#include "Events/EventManager.h"

namespace elv::events {

EventManager gEventManager;

void EventManager::Shutdown()
{
    m_eventsQueue.clear();
    m_subscribers.clear();
    m_subscribersByEventId.clear();
}

void EventManager::Subscribe(EventType eventType, UniquePtr<IEventHandlerWrapper>&& handler, EventId eventId)
{
    if (eventId) {
        auto subscribers = m_subscribersByEventId.find(eventType);

        if (subscribers != m_subscribersByEventId.end()) {
            auto& handlersMap = subscribers->second;
            auto handlers = handlersMap.find(eventId);
            if (handlers != handlersMap.end()) {
                handlers->second.emplace_back(std::move(handler));
                return;
            }
        }
        m_subscribersByEventId[eventType][eventId].emplace_back(std::move(handler));

    } else {
        auto subscribers = m_subscribers.find(eventType);
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
            m_subscribers[eventType].emplace_back(std::move(handler));
        }
    }
}

void EventManager::Unsubscribe(EventType eventType, const std::string& handlerName, EventId eventId)
{
    if (eventId) {
        auto subscribers = m_subscribersByEventId.find(eventType);
        if (subscribers != m_subscribersByEventId.end()) {
            auto& handlersMap = subscribers->second;
            auto handlers = handlersMap.find(eventId);
            if (handlers != handlersMap.end()) {
                auto& callbacks = handlers->second;
                for (auto it = callbacks.begin(); it != callbacks.end(); ++it) {
                    if (it->get()->GetType() == handlerName) {
                        it = callbacks.erase(it);
                        return;
                    }
                }
            }
        }
    } else {
        auto handlersIt = m_subscribers.find(eventType);
        if (handlersIt != m_subscribers.end()) {
            auto& handlers = handlersIt->second;
            for (auto it = handlers.begin(); it != handlers.end(); ++it) {
                if (it->get()->GetType() == handlerName) {
                    it = handlers.erase(it);
                    return;
                }
            }
        }
    }
}

void EventManager::TriggerEvent(const Event& event_, EventId eventId)
{
    for (auto& handler : m_subscribers[event_.GetEventType()]) {
        handler->Exec(event_);
    }

    auto& handlersMap = m_subscribersByEventId[event_.GetEventType()];
    auto handlers = handlersMap.find(eventId);
    if (handlers != handlersMap.end()) {
        auto& callbacks = handlers->second;
        for (auto it = callbacks.begin(); it != callbacks.end();) {
            auto& handler = *it;
            handler->Exec(event_);
            if (handler->IsDestroyOnSuccess()) {
                it = callbacks.erase(it);
            } else {
                ++it;
            }
        }
    }
}

void EventManager::QueueEvent(UniquePtr<Event>&& event_, EventId eventId)
{
    m_eventsQueue.emplace_back(std::move(event_), eventId);
}

void EventManager::DispatchEvents()
{
    for (auto eventIt = m_eventsQueue.begin(); eventIt != m_eventsQueue.end();) {
        if (!eventIt->first.get()->isHandled) {
            TriggerEvent(*eventIt->first.get(), eventIt->second);
            eventIt = m_eventsQueue.erase(eventIt);
        } else {
            ++eventIt;
        }
    }
}

} // namespace elv::events
