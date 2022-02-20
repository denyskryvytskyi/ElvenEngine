#pragma once

#include <unordered_map>
#include <functional>

#define SUBSCRIBE_ON_EVENT(event_uuid, callback) Elven::Events::gEventManager.Subscribe(event_uuid, callback);
#define FIRE_EVENT(event) Elven::Events::gEventManager.Fire(event);

namespace Elven
{
    namespace Events
    {
        class Event;
        class EventManager
        {
            using EventCallback = std::function<void(Event&)>;

        public:
            void Shutdown();

            void Subscribe(const std::string eventId, EventCallback&& callback);
            void Unsubscribe(const std::string eventId, EventCallback&& callback);
            void Fire(Event* event);
            void DispatchEvents();

        private:
            std::vector<Event*> m_eventsQueue;
            std::unordered_map<std::string, std::vector<EventCallback>> m_subscribers;
        };

        extern EventManager gEventManager;
    }
}
