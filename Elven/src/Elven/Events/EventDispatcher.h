#pragma once

#include "Event.h"

namespace Elven
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
            if (m_Event.GetUUID() == T::GetStaticUUID())
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