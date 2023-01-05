#include "EntryPoint.h"

#ifdef EL_PLATFORM_WINDOWS

int main(int argc, char** argv)
{
#    ifdef DEBUG_MODE
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#    endif

    Elven::Log::Init();

    auto app = Elven::CreateApplication();
    app->Run();
    delete app;

    return 0;
}

#endif
