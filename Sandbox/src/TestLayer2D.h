#pragma once

#include <Core/Layer.h>
#include <Events/EventHandler.h>

// forward declarations
namespace Elven::events {
class TextureLoadedEvent;
}
//

class TestLayer2D : public Elven::Layer {
public:
    TestLayer2D();
    virtual ~TestLayer2D();

    virtual void OnAttach() override;
    virtual void OnDetach() override;
    void OnUpdate(float dt) override;
    virtual void OnImGuiRender() override;

private:
    void OnTextureLoaded(const Elven::events::TextureLoadedEvent& e);

private:
    Elven::events::EventHandler<Elven::events::TextureLoadedEvent> m_textureLoadedCallback;
};
