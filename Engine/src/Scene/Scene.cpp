#include "Scene.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Texture2D.h"

#include "Core/Timer.h"

#include "Components/SceneComponents.h"
#include "Systems/BehaviorSystem.h"
#include "Systems/Physics2dSystem.h"
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
    RegisterComponent<AABBComponent>();
    RegisterComponent<TagComponent>();

    // Engine systems register
    RegisterSystem<BehaviorSystem>();
    RegisterSystem<Render2dSystem>();
    RegisterSystem<Physics2dSystem>();
}

void Scene::OnShutdown()
{
    for (auto& system : m_systems) {
        system->OnShutdown();
    }

    for (auto& pool : m_componentPools) {
        pool.second->Clear();
    }

    m_componentPools.clear();
    m_systems.clear();
}

void Scene::OnUpdate(float dt)
{
    for (auto& system : m_systems) {
        system->OnUpdate(dt);
    }

    for (auto entity : m_entitiesToDestroy) {
        DestroyEntity(entity);
    }

    m_entitiesToDestroy.clear();
}

void Scene::OnRender(float dt)
{
    for (auto& system : m_systems) {
        system->OnRender(dt);
    }
}

} // namespace elv
