#pragma once

#include <functional>

#define EVENT_CALLBACK(fn) std::bind(&fn, this, std::placeholders::_1)

namespace Elven
{
    namespace Events
    {
        using EventCallback = std::function<void(Event&)>;

        class Event;
        class EventManager
        {

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

        template<typename EventType>
        static void Subscribe(EventCallback&& callback)
        {
            gEventManager.Subscribe(EventType::GetStaticEventType(), std::move(callback));
        }
        
        template<typename EventType>
        static void Unsubscribe(EventCallback&& callback)
        {
            gEventManager.Unsubscribe(EventType::GetStaticEventType(), std::move(callback));
        }
        
        template<typename TEvent>
        static void TriggerEvent(TEvent triggeredEvent)
        {
            gEventManager.TriggerEvent(triggeredEvent);
        }
        
        template<typename QEvent>
        static void QueueEvent(QEvent queuedEvent)
        {
            gEventManager.QueueEvent(queuedEvent);
        }
    }
}
