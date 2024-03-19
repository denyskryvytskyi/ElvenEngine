#include "MeshModelSandbox.h"
#include "PrimitivesSandbox.h"

elv::Application* elv::CreateApplication()
{
    return new MeshModelSandbox();
}
