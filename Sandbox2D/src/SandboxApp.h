#pragma once

// forward declarations
namespace Elven::events {
class TextureLoadedEvent;
}

class Sandbox2D : public Elven::Application {
public:
    Sandbox2D();

    virtual void OnCreate();
    virtual void OnUpdate(float dt);
    virtual void OnRender(float dt);
    virtual void OnDestroy();

private:
    void OnTextureLoaded(const Elven::events::TextureLoadedEvent& e);

private:
    Elven::events::EventHandler<Elven::events::TextureLoadedEvent> m_textureLoadedCallback;
};
