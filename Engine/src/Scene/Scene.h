#pragma once

#include "Events/EventHandler.h"
#include "Renderer/OrthographicCameraController.h"
#include "Scene/World.h"

namespace Elven {

// forward declarations
namespace events {
class TextureLoadedEvent;
}

struct SceneNodeComponent {
    std::vector<UniquePtr<SceneNodeComponent>> m_childs;
    ecs::EntityId m_entityId;
};

class Scene {
public:
    Scene();

    void Init();
    void Shutdown();
    void OnUpdate(float dt);
    void OnRender(float dt);

private:
    void OnTextureLoaded(const events::TextureLoadedEvent& e);

private:
    SceneNodeComponent root;
    ecs::World m_world;
    Elven::OrthographicCameraController m_cameraController;

    events::EventHandler<events::TextureLoadedEvent> m_textureLoadedCallback;
};

} // namespace Elven
