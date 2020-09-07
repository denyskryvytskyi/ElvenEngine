#pragma once

#include <string>
#include <iostream>

#include "Elven/Core/Core.h"

namespace Elven
{
    // Events are currently blocking, 
    // when an event occurs it immediately gets dispatched and must be dealt with right then an there.
    // For the future, a better strategy might be to buffer events in an event
    // bus and process them during the "event" part of the update stage.

    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize, WindowLostFocus, WindowMoved,
        AppTick, AppUpdate, AppRender,
        KeyPressed, KeyReleased, KeyTyped,
        MouseButtonPressed, MouseButtonReleased, MouseMoved, MouseScrolled
    };

    enum EventCategory
    {
        None = 0,
        EventCategory_Application   = BIT(0),
        EventCategory_Input         = BIT(1),
        EventCategory_Keybord       = BIT(2),
        EventCategory_Mouse         = BIT(3),
        EventCategory_MouseButton   = BIT(4)
    };

    #define EVENT_CLASS_TYPE(type) static EventType GetStaticType() { return EventType::type; }\
                                   virtual EventType GetEventType() const override { return GetStaticType(); } \
                                   virtual const char* GetName() const override { return #type; }

    #define EVENT_CLASS_CATEGORY(category) virtual int GetCategoryFlags() const override { return category; }

    class Event
    {
    public:
        bool Handled = false;

    public:
        virtual EventType GetEventType() const = 0;
        virtual const char* GetName() const = 0;
        virtual int GetCategoryFlags() const = 0;
        virtual std::string ToString() const { return GetName(); }

        bool IsInCategory(EventCategory category)
        {
            return GetCategoryFlags() & category;
        }
    };

    class EventDispatcher
    {
        template<typename T>
        using EventFn = std::is_function<bool(T&)>;

    public:
        EventDispatcher(Event& event)
            : m_Event(event) {}

        template<typename T>
        bool Dispatch(EventFn<T> func)
        {
            if (m_Event.GetEventType() == T::GetStaticType())
            {
                m_Event.Handled = func(*(T*)&m_Event);
            }
        }

    private:
        Event& m_Event;
    };

    inline std::ostream& operator <<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }
}