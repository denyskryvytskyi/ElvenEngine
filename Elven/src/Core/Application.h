#pragma once

#include "Core.h"

namespace Elven
{
    class ELVEN_API Application
    {
    public:
        Application();
        virtual ~Application();
        //
        void Run();
    };

    // To be defined in CLIENT
    Application* CreateApplication();
}


