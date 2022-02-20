#pragma once

#ifdef EL_PLATFORM_WINDOWS

extern Elven::Application* Elven::CreateApplication();

int main(int argc, char** argv)
{
    Elven::Log::Init();

    EL_CORE_WARN("Init");
    int a = 10;
    EL_INFO("Var={0}", a);


    auto app = Elven::CreateApplication();
    app->Run();
    SafePointerDelete(app);
}

#endif