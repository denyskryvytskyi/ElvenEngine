#pragma once

// forward declarations
namespace Elven::events {
class TextureLoadedEvent;
}

class Sandbox2D : public Elven::Application {
public:
    Sandbox2D();

    void OnCreate() override;
    void OnUpdate(float dt) override;
    void OnRender(float dt) override;
    void OnDestroy() override;

private:
    void OnTextureLoaded(const Elven::events::TextureLoadedEvent& e);

private:
    Elven::events::EventHandler<Elven::events::TextureLoadedEvent> m_textureLoadedCallback;
};
