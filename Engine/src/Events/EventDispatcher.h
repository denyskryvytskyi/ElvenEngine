#pragma once

#include "Event.h"
#include "EventManager.h"

namespace Elven
{
    namespace Events
    {
        class EventDispatcher
        {
        public:
            EventDispatcher(Event& event) :
                m_Event(event)
            {}

            template<typename EventType, typename FuncType>
            bool Dispatch(const FuncType& func)
            {
                if (m_Event.GetEventType() == EventType::GetStaticEventType())
                {
                    m_Event.Handled = func(static_cast<EventType&>(m_Event));
                    return true;
                }
                return false;
            }

        private:
            Event& m_Event;
        };
    }
}