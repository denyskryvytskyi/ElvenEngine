#pragma once

#include <Elven.h>
#include "ExampleLayer.h"

class Sandbox : public Elven::Application
{
public:
    Sandbox()
    {
        PushLayer(new ExampleLayer());
    }
};