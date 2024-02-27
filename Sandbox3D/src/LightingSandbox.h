#pragma once

#include <Elven.h>
#include <Renderer/VertexArray.h>

// forward declarations
namespace elv::events {
class TextureLoadedEvent;
}

class LightingSandbox : public elv::Application {
public:
    LightingSandbox();

    virtual void OnCreate();
    virtual void OnUpdate(float dt);
    virtual void OnRender(float dt);
    virtual void OnImguiRender();
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

    std::vector<lia::vec3> m_cubes;

    // light
    elv::SharedPtr<elv::VertexArray> m_lightVao;
    elv::SharedPtr<elv::Shader> m_lightShader;

    lia::vec3 m_lightPos { 0.8f, 1.0f, 1.2f };
};
