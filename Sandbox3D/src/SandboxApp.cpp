#include "CubesSandbox.h"
#include "LightingSandbox.h"

elv::Application* elv::CreateApplication()
{
    return new LightingSandbox();
}
