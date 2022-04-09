#include "SandboxApp.h"

#include <Core/EntryPoint.h>


Elven::Application* Elven::CreateApplication()
{
    return new Sandbox();
}