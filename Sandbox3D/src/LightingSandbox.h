#pragma once

#include <Elven.h>
#include <Renderer/VertexArray.h>

// forward declarations
namespace elv::events {
class TextureLoadedEvent;
}

struct Material {
    lia::vec3 ambient { 1.0f, 0.5f, 0.31f };
    lia::vec3 diffuse { 1.0f, 0.5f, 0.31f };
    lia::vec3 specular { 0.5f, 0.5f, 0.5f };
    float shininess { 32.0f };
};

struct Light {
    lia::vec3 position { 0.9f, 0.8f, 1.2f };

    lia::vec3 ambient { 0.2f, 0.2f, 0.2f };
    lia::vec3 diffuse { 0.4f, 0.4f, 0.4f };
    lia::vec3 specular { 1.0f, 1.0f, 1.0f };
};

class LightingSandbox : public elv::Application {
public:
    LightingSandbox();

    virtual void OnCreate();
    virtual void OnUpdate(float dt);
    virtual void OnRender(float dt);
#if EDITOR_MODE
    virtual void OnImguiRender();
#endif
    virtual void OnDestroy();

private:
    void OnTextureLoaded(const elv::events::TextureLoadedEvent& e);

private:
    elv::EditorCameraController m_cameraController;
    elv::events::EventHandler<elv::events::TextureLoadedEvent> m_textureLoadedCallback;

    // objects
    elv::SharedPtr<elv::VertexArray> m_vao;
    elv::SharedPtr<elv::Shader> m_shader;
    elv::SharedPtr<elv::Texture2D> m_texture;

    lia::vec3 m_cubePosition;
    lia::vec3 m_cubeRotation;
    lia::vec3 m_cubeScale { 1.0f };

    Material m_cubeMaterial;

    // light
    elv::SharedPtr<elv::VertexArray> m_lightVao;
    elv::SharedPtr<elv::Shader> m_lightShader;

    Light m_light;
};
