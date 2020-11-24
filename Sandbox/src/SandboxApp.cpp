#include "SandboxApp.h"

#include <Elven/Core/EntryPoint.h>

Elven::Application* Elven::CreateApplication()
{
    return new Sandbox();
}