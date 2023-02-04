#include "Scene.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Texture2D.h"

#include "Core/Timer.h"

#include "Components/SceneComponents.h"
#include "Systems/BehaviorSystem.h"
#include "Systems/Render2dSystem.h"

namespace elv {

Scene::Scene()
{
}

void Scene::OnInit()
{
    // Engine components register
    RegisterComponent<TransformComponent>();
    RegisterComponent<SpriteComponent>();
    RegisterComponent<BehaviorComponent>();
    RegisterComponent<QuadComponent>();
    RegisterComponent<CameraComponent>();
    RegisterComponent<TextComponent>();
    RegisterComponent<RectTransformComponent>();

    // Engine systems register
    RegisterSystem<BehaviorSystem>();
    RegisterSystem<Render2dSystem>();
}

void Scene::OnShutdown()
{
    for (auto& system : m_systems) {
        system->OnShutdown();
    }

    m_componentPools.clear();
    m_entities.clear();
    m_systems.clear();
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

} // namespace elv
