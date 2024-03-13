#pragma once

#include <Elven.h>
#include <Renderer/VertexArray.h>

#include <Renderer/Model.h>

// forward declarations
namespace elv::events {
class TextureLoadedEvent;
}

namespace meshModel {
const int kPointLightsAmount = 4;
}

class MeshModelSandbox : public elv::Application {
public:
    MeshModelSandbox();

    virtual void OnUpdate(float dt);
    virtual void OnRender(float dt);
#if EDITOR_MODE
    virtual void OnImguiRender();
#endif

private:
    void OnTextureLoaded(const elv::events::TextureLoadedEvent& e);

    void SetEnvironment(const int envIndex);

    void SetupCubeMesh();

private:
    elv::EditorCameraController m_cameraController;
    elv::events::EventHandler<elv::events::TextureLoadedEvent> m_textureLoadedCallback;

    elv::Timer m_timer;

    lia::vec4 m_clearColor { 0.2f, 0.2f, 0.2f, 1.0f };

    int m_texturesToLoad { 3 };
    int m_texturesLoaded { 0 };
    bool m_texturesIsReady { false };

    bool m_DirLightEnabled { true };
    bool m_PointLightEnabled { false };
    bool m_SpotLightEnabled { false };

    // objects
    std::vector<elv::ecs::Entity> m_cubes;

    float m_cubeShininess { 1.0f };

    elv::SharedPtr<elv::Shader> m_shader { nullptr };

    // mesh
    elv::UniquePtr<elv::Mesh> m_cubeMesh { nullptr };

    // light
    elv::PointLight m_pointLights[meshModel::kPointLightsAmount];
    elv::DirectionalLight m_dirLight;
    elv::SpotLight m_flashlight;

    /*elv::SharedPtr<elv::VertexArray> m_lightVao { nullptr };*/
    elv::UniquePtr<elv::Mesh> m_lightCubeMesh { nullptr };
    elv::SharedPtr<elv::Shader> m_lightShader { nullptr };

    // Model
    elv::Model m_model;
    elv::ecs::Entity m_backpack;
};
