#pragma once

#include "TestLayer2D.h"
#include "TestLayer3D.h"
#include <Elven.h>

class Sandbox : public Elven::Application {
public:
    Sandbox()
    {
        PushLayer(new TestLayer2D());
    }
};
