#pragma once

#include <Elven.h>

class ExampleLayer : public Elven::Layer
{
public:
    ExampleLayer();
    virtual ~ExampleLayer();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    void OnUpdate(float dt) override;
    virtual void OnImGuiRender() override;

private:
    Elven::OrthographicCameraController m_CameraController;
    Elven::SharedPtr<Elven::Texture2D> m_texture;
};