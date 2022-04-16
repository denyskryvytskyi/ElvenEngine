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

            template<typename T, typename F>
            bool Dispatch(const F& func)
            {
                if (m_Event.GetEventType() == T::GetStaticEventType())
                {
                    m_Event.Handled = func(*(T*)&m_Event);
                    return true;
                }
                return false;
            }

        private:
            Event& m_Event;
        };
    }
}