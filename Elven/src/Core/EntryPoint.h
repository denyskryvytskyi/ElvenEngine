#pragma once

#ifdef EV_PLATFORM_WINDOWS

extern Elven::Application* Elven::CreateApplication();

int main(int argc, char** argv)
{
    printf("Elven Engine\n");
    auto app = Elven::CreateApplication();
    app->Run();
    delete app;
}

#endif