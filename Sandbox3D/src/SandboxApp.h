#pragma once

#include <Elven.h>
#include <Renderer/VertexArray.h>

// forward declarations
namespace Elven::events {
class TextureLoadedEvent;
}

class Sandbox3D : public Elven::Application {
public:
    Sandbox3D();

    virtual void OnCreate();
    virtual void OnUpdate(float dt);
    virtual void OnRender(float dt);
    virtual void OnDestroy();

private:
    void OnTextureLoaded(const Elven::events::TextureLoadedEvent& e);

private:
    Elven::CameraController m_cameraController;

    Elven::SharedPtr<Elven::VertexArray> m_vao;
    Elven::SharedPtr<Elven::Shader> m_shader;

    Elven::SharedPtr<Elven::Texture2D> m_texture;

    std::vector<lia::vec3> m_cubes;

    Elven::events::EventHandler<Elven::events::TextureLoadedEvent> m_textureLoadedCallback;
};
