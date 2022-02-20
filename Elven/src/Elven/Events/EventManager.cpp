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
                SafePointerDelete(event);
            }
        }

        void EventManager::Subscribe(const std::string eventId, EventCallback&& callback)
        {
            auto callbacks = m_subscribers.find(eventId);
            if (callbacks != m_subscribers.end())
            {
                callbacks->second.emplace_back(std::move(callback));
            }
            else
            {
                m_subscribers[eventId].emplace_back(callback);
            }
        }

        void EventManager::Unsubscribe(const std::string eventId, EventCallback&& callback)
        {
            const size_t function_adress = *reinterpret_cast<long*>(reinterpret_cast<char*>(&callback));
            auto& callbacks = m_subscribers[eventId];

            for (auto it = m_subscribers.begin(); it != m_subscribers.end();)
            {
                const size_t callback_adress = *reinterpret_cast<long*>(reinterpret_cast<char*>(&(*it)));
                if (callback_adress == function_adress)
                {
                    it = m_subscribers.erase(it);
                    return;
                }
            }
        }

        void EventManager::Fire(Event* event)
        {
            m_eventsQueue.emplace_back(event);
        }

        void EventManager::DispatchEvents()
        {
            for (auto eventIt = m_eventsQueue.begin(); eventIt != m_eventsQueue.end();)
            {
                if (!(*eventIt)->Handled)
                {
                    for (auto callback : m_subscribers[(*eventIt)->GetUUID()])
                    {
                        callback(*(*eventIt));
                    }
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