#pragma once
#include <unordered_map>
#include "Elven/Events/Event.h"
namespace Elven
{
    class EventManager
    {
        using EventCallback = std::function<void(Event&)>;

    public:
        void Shutdown();

        void Subscribe(const std::string eventId, EventCallback&& callback);
        void Fire(Event* event);
        void DispatchEvents();

    private:
        std::vector<Event*> m_eventsQueue;
        std::unordered_map<std::string, std::vector<EventCallback>> m_callbacks;
    };

    extern EventManager gEventManager;
}
