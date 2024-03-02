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

struct PointLight {
    lia::vec3 position { 0.0f, 0.8f, 1.2f };

    lia::vec3 ambient { 0.2f, 0.2f, 0.2f };
    lia::vec3 diffuse { 0.4f, 0.4f, 0.4f };
    lia::vec3 specular { 1.0f, 1.0f, 1.0f };

    // distance 160
    float constant { 1.0f };
    float linear { 0.027f };
    float quadratic { 0.0028f };
};

struct DirectionalLight {
    lia::vec3 direction { -0.2f, -1.0f, -0.3f };

    lia::vec3 ambient { 0.2f, 0.2f, 0.2f };
    lia::vec3 diffuse { 0.4f, 0.4f, 0.4f };
    lia::vec3 specular { 1.0f, 1.0f, 1.0f };
};

struct SpotLight {
    lia::vec3 position;
    lia::vec3 direction;

    lia::vec3 ambient { 0.2f, 0.2f, 0.2f };
    lia::vec3 diffuse { 0.4f, 0.4f, 0.4f };
    lia::vec3 specular { 1.0f, 1.0f, 1.0f };

    float cutOff { 12.5f };
    float outerCutOff { 17.5f };
    // distance 160
    float constant { 1.0f };
    float linear { 0.027f };
    float quadratic { 0.0028f };
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

    elv::Timer m_timer;

    int m_texturesToLoad { 3 };
    int m_texturesLoaded { 0 };
    bool m_texturesIsReady { false };

    bool m_lightDemoRotationEnabled { false };

    // objects
    lia::vec3 m_cubePosition;
    lia::vec3 m_cubeRotation;
    lia::vec3 m_cubeScale { 1.0f };

    Material m_cubeMaterial;

    elv::SharedPtr<elv::VertexArray> m_vao { nullptr };
    elv::SharedPtr<elv::Shader> m_shader { nullptr };

    // light
    PointLight m_pointLight;
    DirectionalLight m_dirLight;
    SpotLight m_flashlight;

    elv::SharedPtr<elv::VertexArray> m_lightVao { nullptr };
    elv::SharedPtr<elv::Shader> m_lightShader { nullptr };
};
