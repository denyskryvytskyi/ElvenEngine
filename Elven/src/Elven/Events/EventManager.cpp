#include "elpch.h"
#include "Elven/Events/Event.h"
#include "Elven/Events/EventManager.h"

namespace Elven
{
    namespace Events
    {
        EventManager gEventManager;

        void EventManager::Shutdown()
        {
            for (auto event : m_eventsQueue)
            {
                delete event;
            }
        }

        void EventManager::Subscribe(const std::string eventId, EventCallback&& callback)
        {
            auto subscribers = m_subscribers.find(eventId);
            if (subscribers != m_subscribers.end())
            {
                auto& callbacks = subscribers->second;
                for (auto it = callbacks.begin(); it != callbacks.end(); ++it)
                {
                    if (it->target_type().name() == callback.target_type().name())
                    {
                        EL_ASSERT(false, "Attempting to double-register callback");
                        return;
                    }
                }
                callbacks.emplace_back(std::move(callback));
            }
            else
            {
                m_subscribers[eventId].emplace_back(std::move(callback));
            }
        }

        void EventManager::Unsubscribe(const std::string eventId, EventCallback&& callback)
        {
            auto& callbacks = m_subscribers[eventId];
            for (auto it = callbacks.begin(); it != callbacks.end(); ++it)
            {
                if (it->target_type().name() == callback.target_type().name())
                {
                    it = callbacks.erase(it);
                    return;
                }
            }
        }

        void EventManager::TriggerEvent(Event* event)
        {
            for (auto callback : m_subscribers[event->GetUUID()])
            {
                callback(*event);
            }
        }

        void EventManager::QueueEvent(Event* event)
        {
            m_eventsQueue.emplace_back(event);
        }

        void EventManager::DispatchEvents()
        {
            for (auto eventIt = m_eventsQueue.begin(); eventIt != m_eventsQueue.end();)
            {
                if (!(*eventIt)->Handled)
                {
                    TriggerEvent(*eventIt);
                    eventIt = m_eventsQueue.erase(eventIt);
                }
                else
                {
                    ++eventIt;
                }
            }
        }
    }
}