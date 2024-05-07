#include "EntryPoint.h"

int main(int argc, char** argv)
{
#ifdef DEBUG_MODE and _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

    srand(static_cast<uint32_t>(time(0)));

    elv::Log::Init();

    auto app = elv::CreateApplication();
    app->Run();
    delete app;

    return 0;
}