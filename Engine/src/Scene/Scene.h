#pragma once
#include "World.h"

#include "Events/EventManager.h"
#include "Renderer/OrthographicCameraController.h"

namespace Elven {

namespace events {
class TextureLoadedEvent;
}

struct SceneNode {
    std::vector<UniquePtr<SceneNode>> m_childs;
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
    SceneNode root;
    ecs::World m_world;
    Elven::OrthographicCameraController m_cameraController;

    events::EventFunctionHandler<events::TextureLoadedEvent> m_textureLoadedCallback;
};

} // namespace Elven
