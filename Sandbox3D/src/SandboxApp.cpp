#include "CubesSandbox.h"
#include "LightCastersSandbox.h"
#include "SimpleLightSandbox.h"

elv::Application* elv::CreateApplication()
{
    return new LightCastersSandbox();
}
