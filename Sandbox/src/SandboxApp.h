#pragma once

#include <Elven.h>
#include "TestLayer2D.h"
#include "TestLayer3D.h"

class Sandbox : public Elven::Application
{
public:
    Sandbox()
    {
        PushLayer(new TestLayer3D());
    }
};