#pragma once

#include <Elven.h>
#include <Renderer/VertexArray.h>

// simple position light
struct Light {
    lia::vec3 position { 0.0f, 0.8f, 1.2f };

    lia::vec3 ambient { 0.2f, 0.2f, 0.2f };
    lia::vec3 diffuse { 0.4f, 0.4f, 0.4f };
    lia::vec3 specular { 1.0f, 1.0f, 1.0f };
};

class SimpleLightSandbox : public elv::Application {
public:
    SimpleLightSandbox();

    void OnUpdate(float dt) override;
    void OnRender(float dt) override;
    void OnProcessInput(float dt) override;
#if EDITOR_MODE
    void OnImguiRender() override;
#endif

private:
    elv::EditorCameraController m_cameraController;
    elv::Timer m_timer;

    // cube
    elv::SharedPtr<elv::VertexArray> m_vao;
    elv::SharedPtr<elv::Shader> m_shader;
    elv::SharedPtr<elv::Texture2D> m_texture;

    lia::vec3 m_cubePosition;
    lia::vec3 m_cubeRotation;
    lia::vec3 m_cubeScale { 1.0f };

    elv::Material m_cubeMaterial;

    elv::ecs::Entity m_cubeEntity;

    // light
    elv::SharedPtr<elv::VertexArray> m_lightVao;
    elv::SharedPtr<elv::Shader> m_lightShader;

    elv::ecs::Entity m_lightEntity;

    Light m_light;

    // settings
    bool m_lightDemoRotationEnabled { false };
};
