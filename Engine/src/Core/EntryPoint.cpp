#include "EntryPoint.h"

#ifdef EL_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
    Elven::Log::Init();

    auto app = Elven::CreateApplication();
    app->Run();
    delete app;
}

#endif
