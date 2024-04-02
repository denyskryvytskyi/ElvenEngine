#include "Scene.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Texture2D.h"

#include "Core/Profiler.h"

#include "Components/LightComponent.h"
#include "Components/SceneComponents.h"
#include "Components/StaticMeshComponent.h"
#include "Systems/BehaviorSystem.h"
#include "Systems/LightSystem.h"
#include "Systems/Physics2dSystem.h"
#include "Systems/Render2dSystem.h"
#include "Systems/RenderSystem.h"

#include "Core/SettingsConfig.h"

namespace elv {
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
    RegisterComponent<PointLightComponent>();
    RegisterComponent<DirectionalLightComponent>();
    RegisterComponent<SpotLightComponent>();
    RegisterComponent<StaticMeshComponent>();
    RegisterComponent<SoundComponent>();
    if (gEngineSettings.enableSceneGraph) {
        RegisterComponent<SceneNodeComponent>();
    }

    // Engine systems register
    RegisterSystem<BehaviorSystem>();
#if ASSIMP_MODE
    RegisterSystem<RenderSystem>();
#endif
    RegisterSystem<Render2dSystem>();
    RegisterSystem<Physics2dSystem>();
    RegisterSystem<LightSystem>();

    if (gEngineSettings.enableSceneGraph) {
        m_sceneGraph.OnInit(this);

        m_sceneGraphRoot = CreateEntity();
        AddComponent<SceneNodeComponent>(m_sceneGraphRoot);
        AddComponent<TagComponent>(m_sceneGraphRoot, "Root");
    }
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
    {
        PROFILE_SCOPE("Scene updated in: ");
        for (auto& system : m_systems) {
            system->OnUpdate(dt);
        }
    }

    for (auto entity : m_entitiesToDestroy) {
        DestroyEntity(entity);
    }

    m_entitiesToDestroy.clear();

    if (gEngineSettings.enableSceneGraph && m_sceneGraphRoot != ecs::INVALID_ENTITY_ID) {
        m_sceneGraph.OnUpdate(this);
    }
}

void Scene::OnRender(float dt)
{
    PROFILE("Scene rendered in: ");
    for (auto& system : m_systems) {
        system->OnRender(dt);
    }
}

ecs::Entity Scene::CreateEntity()
{
    if (gEngineSettings.enableSceneGraph && m_sceneGraphRoot != ecs::INVALID_ENTITY_ID) {
        return CreateChildEntity(m_sceneGraphRoot);
    }

    const ecs::Entity id = ecs::GenerateEntityId();
    m_entities.insert({ id, ecs::ComponentMask() });

    return id;
}

ecs::Entity Scene::CreateChildEntity(ecs::Entity parentEntity)
{
    ecs::Entity id = ecs::INVALID_ENTITY_ID;
    EL_CORE_ASSERT(gEngineSettings.enableSceneGraph, "Failed to create child entity. Scene Graph is disabled. Please turn it on in settings");

    if (gEngineSettings.enableSceneGraph) {
        auto parentEntityIt = m_entities.find(parentEntity);

        if (parentEntityIt != m_entities.end()) {
            id = ecs::GenerateEntityId();
            m_entities.insert({ id, ecs::ComponentMask() });

            AddComponent<SceneNodeComponent>(id, parentEntity);

        } else {
            EL_CORE_WARN("Entity isn't exist, can't add child to it");
        }
    } else {
        EL_CORE_ERROR("Failed to create child entity. Scene Graph is disabled. Please turn it on in settings");
    }

    return id;
}

void Scene::DestroyEntity(ecs::Entity entity)
{
    auto entityIt = m_entities.find(entity);

    if (entityIt != m_entities.end()) {
        const auto& componentMask = entityIt->second;
        for (size_t i = 0; i < componentMask.size(); ++i) {
            if (componentMask.test(i)) {
                m_componentPools[i]->RemoveComponent(entity);
            }
        }
        if (gEngineSettings.enableSceneGraph) {
            std::vector<ecs::Entity> childrenToDestroy;

            const auto sceneNodesPool = GetComponentPool<SceneNodeComponent>();
            const auto& sceneNodeComponents = sceneNodesPool->GetComponents();

            for (uint32_t index = 0; index < sceneNodeComponents.size(); ++index) {
                auto& node = sceneNodeComponents.at(index);
                if (node.parent == entity) {
                    childrenToDestroy.emplace_back(sceneNodesPool->GetEntity(index));
                }
            }
            for (auto& child : childrenToDestroy) {
                DestroyEntity(child);
            }
        }

        m_entities.erase(entityIt);
    }
}

} // namespace elv
