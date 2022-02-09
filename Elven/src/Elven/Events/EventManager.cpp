#include "elpch.h"
#include "EventManager.h"

namespace Elven
{
    EventManager gEventManager;

    void EventManager::Shutdown()
    {
        for (auto event : m_eventsQueue)
        {
            DeleteRawPointer(event);
        }
    }

    void EventManager::Subscribe(const std::string eventId, EventCallback&& callback)
    {
        auto subscribedCallbacks = m_callbacks.find(eventId);
        if (subscribedCallbacks != m_callbacks.end())
        {
            subscribedCallbacks->second.emplace_back(std::move(callback));
        }
        else
        {
            m_callbacks[eventId].emplace_back(callback);
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
                for (auto callback : m_callbacks[(*eventIt)->GetUUID()])
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
