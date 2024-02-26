#pragma once

#include <Elven.h>
#include <Renderer/VertexArray.h>

// forward declarations
namespace elv::events {
class TextureLoadedEvent;
}

class CubesSandbox : public elv::Application {
public:
    CubesSandbox();

    virtual void OnCreate();
    virtual void OnUpdate(float dt);
    virtual void OnRender(float dt);
    virtual void OnDestroy();

private:
    void OnTextureLoaded(const elv::events::TextureLoadedEvent& e);

private:
    elv::CameraController m_cameraController;

    elv::SharedPtr<elv::VertexArray> m_vao;
    elv::SharedPtr<elv::Shader> m_shader;

    elv::SharedPtr<elv::Texture2D> m_texture;

    std::vector<lia::vec3> m_cubes;

    elv::events::EventHandler<elv::events::TextureLoadedEvent> m_textureLoadedCallback;

    elv::Timer m_timer;
};
