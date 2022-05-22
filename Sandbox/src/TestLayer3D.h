#pragma once

#include <Elven.h>
#include <Renderer/VertexArray.h>

class TestLayer3D : public Elven::Layer
{
public:
    TestLayer3D();
    virtual ~TestLayer3D();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    void OnUpdate(float dt) override;
    virtual void OnImGuiRender() override;

private:
    Elven::CameraController m_cameraController;

    Elven::VertexArray* m_vao;
    Elven::Shader* m_shader;

    Elven::SharedPtr<Elven::Texture2D> m_texture;

    std::vector<lia::vec3> m_cubes;

};

