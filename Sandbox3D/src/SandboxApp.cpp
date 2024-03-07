#include "CubesSandbox.h"
#include "LightCastersSandbox.h"
#include "MeshModelSandbox.h"
#include "SimpleLightSandbox.h"

elv::Application* elv::CreateApplication()
{
    return new MeshModelSandbox();
}
