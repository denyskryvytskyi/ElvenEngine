#pragma once

#include <Elven.h>

class TestLayer2D : public Elven::Layer {
public:
    TestLayer2D();
    virtual ~TestLayer2D();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    void OnUpdate(float dt) override;
    virtual void OnImGuiRender() override;
};
