#include "Scene.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Texture2D.h"

#include "Core/Timer.h"

#include "Components/SceneComponents.h"
#include "Systems/BehaviorSystem.h"
#include "Systems/SpriteRenderSystem.h"

namespace Elven {

Scene::Scene()
{
}

void Scene::OnInit()
{
    // Engine components register
    RegisterComponent<TransformComponent>();
    RegisterComponent<SpriteComponent>();
    RegisterComponent<BehaviorComponent>();

    // Engine systems register
    RegisterSystem<BehaviorSystem>();
    RegisterSystem<SpriteRenderSystem>();
}

void Scene::OnShutdown()
{
    m_componentPools.clear();
    m_entities.clear();
}

void Scene::OnUpdate(float dt)
{
    for (auto& system : m_systems) {
        system->OnUpdate(dt);
    }
}

void Scene::OnRender(float dt)
{
    for (auto& system : m_systems) {
        system->OnRender(dt);
    }
}

} // namespace Elven
