#pragma once

#include <unordered_map>
#include <functional>

#define SUBSCRIBE_ON_EVENT(event_uuid, callback) Elven::Events::gEventManager.Subscribe(event_uuid, callback);
#define UNSUBSCRIBE_EVENT(event_uuid, callback) Elven::Events::gEventManager.Unsubscribe(event_uuid, callback);
#define TRIGGER_EVENT(event) Elven::Events::gEventManager.TriggerEvent(event);
#define QUEUE_EVENT(event) Elven::Events::gEventManager.QueueEvent(event);

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
            void TriggerEvent(Event* event);
            void QueueEvent(Event* event);
            void DispatchEvents();

        private:
            std::vector<Event*> m_eventsQueue;
            std::unordered_map<std::string, std::vector<EventCallback>> m_subscribers;
        };

        extern EventManager gEventManager;
    }
}
