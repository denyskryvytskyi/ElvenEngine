#pragma once

#include <string>
#include <sstream>

//#include "Elven/Core/Core.h"

namespace Elven
{
    class Event
    {
    public:
        virtual std::string GetUUID() const = 0;
        virtual std::string ToString() const { return GetUUID(); };

    public:
        bool Handled = false;
    };

    #define EVENT_ID(str_id) static const std::string GetStaticUUID() { return str_id; } \
                             virtual std::string GetUUID() const { return GetStaticUUID(); }

    inline std::ostream& operator <<(std::ostream& os, const Event& e)
    {
        return os << e.ToString();
    }
}