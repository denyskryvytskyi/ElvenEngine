#pragma once

// forward declarations
namespace elv::events {
class TextureLoadedEvent;
}

class Sandbox2D : public elv::Application {
public:
    Sandbox2D();

    void OnCreate() override;
    void OnUpdate(float dt) override;
    void OnRender(float dt) override;
    void OnDestroy() override;

private:
    void OnTextureLoaded(const elv::events::TextureLoadedEvent& e);

private:
    elv::events::EventHandler<elv::events::TextureLoadedEvent> m_textureLoadedCallback;
};
