#pragma once

#include <Elven.h>
#include <Renderer/VertexArray.h>

#include "Helpers/EnvironmentSettings.h"

// forward declarations
namespace elv::events {
class TextureLoadedEvent;
}

class LightCastersSandbox : public elv::Application {
public:
    LightCastersSandbox();

    virtual void OnUpdate(float dt);
    virtual void OnRender(float dt);
#if EDITOR_MODE
    virtual void OnImguiRender();
#endif

private:
    void OnTextureLoaded(const elv::events::TextureLoadedEvent& e);

    void SetEnvironment(const int envIndex);

private:
    elv::EditorCameraController m_cameraController;
    elv::events::EventHandler<elv::events::TextureLoadedEvent> m_textureLoadedCallback;

    elv::Timer m_timer;

    lia::vec4 m_clearColor { 0.2f, 0.2f, 0.2f, 1.0f };

    int m_texturesToLoad { 3 };
    int m_texturesLoaded { 0 };
    bool m_texturesIsReady { false };

    bool m_DirLightEnabled { false };
    bool m_PointLightEnabled { false };
    bool m_SpotLightEnabled { false };

    // objects
    lia::vec3 m_cubePosition;
    lia::vec3 m_cubeRotation;
    lia::vec3 m_cubeScale { 1.0f };

    float m_cubeShininess { 32.0f };

    elv::SharedPtr<elv::VertexArray> m_vao { nullptr };
    elv::SharedPtr<elv::Shader> m_shader { nullptr };

    // light
    elv::PointLight m_pointLights[kPointLightsAmount];
    elv::DirectionalLight m_dirLight;
    elv::SpotLight m_flashlight;

    elv::SharedPtr<elv::VertexArray> m_lightVao { nullptr };
    elv::SharedPtr<elv::Shader> m_lightShader { nullptr };
};
