#include "Scene.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer2D.h"
#include "Renderer/Texture2D.h"

#include "Core/Profiler.h"

#include "Components/SceneComponents.h"
#include "Systems/BehaviorSystem.h"
#include "Systems/Physics2dSystem.h"
#include "Systems/Render2dSystem.h"

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

    // Engine systems register
    RegisterSystem<BehaviorSystem>();
    RegisterSystem<Render2dSystem>();
    RegisterSystem<Physics2dSystem>();

    // root entity
    if (gEngineSettings.enableSceneGraph) {
        AddComponent<TransformComponent>(m_root->entityId);
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
        // PROFILE_SCOPE("Scene updated in: ");
        for (auto& system : m_systems) {
            system->OnUpdate(dt);
        }
    }

    for (auto entity : m_entitiesToDestroy) {
        DestroyEntity(entity);
    }

    m_entitiesToDestroy.clear();
}

void Scene::OnRender(float dt)
{
    // iterate scene graph to update transform (model matrix) and children
    {
        // PROFILE_SCOPE("Scene graph updated in: ");
        if (gEngineSettings.enableSceneGraph && m_root) {
            m_root->UpdateTransform(this, false);
        }
    }

    // render
    {
        // PROFILE_SCOPE("Scene rendered in: ");
        for (auto& system : m_systems) {
            system->OnRender(dt);
        }
    }
}

ecs::Entity Scene::CreateEntity()
{
    EntityInfo entityInfo;

    if (gEngineSettings.enableSceneGraph) {
        if (!m_root) {
            const ecs::Entity rootId = ecs::GenerateEntityId();
            m_root = MakeSharedPtr<SceneNode>(rootId);
            entityInfo.node = m_root;
            m_entities.insert({ rootId, entityInfo });
        }
        return CreateChildEntity(m_root->entityId);
    }

    const ecs::Entity id = ecs::GenerateEntityId();
    m_entities.insert({ id, entityInfo });

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
            auto child = MakeSharedPtr<SceneNode>(id);

            // add child to parent
            parentEntityIt->second.node->AddChild(child);

            // sertup child
            EntityInfo entityInfo;
            entityInfo.node = child;
            entityInfo.node->parent = parentEntityIt->second.node;
            m_entities.insert({ id, entityInfo });

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
        const auto& componentMask = entityIt->second.mask;
        for (size_t i = 0; i < componentMask.size(); ++i) {
            if (componentMask.test(i)) {
                m_componentPools[i]->RemoveComponent(entity);
            }
        }
        if (gEngineSettings.enableSceneGraph) {
            const auto& sceneNode = entityIt->second.node;
            for (auto& child : sceneNode->children) {
                DestroyEntity(child->entityId);
            }
            if (sceneNode->parent) {
                sceneNode->parent->RemoveChild(entity);
            }
        }

        m_entities.erase(entityIt);
    }
}

Scene::SceneNode::SceneNode(ecs::Entity entity)
    : entityId(entity)
{
}

void Scene::SceneNode::UpdateTransform(Scene* scene, bool isParentDirty)
{
    if (!scene->HasComponent<TransformComponent>(entityId)) {
        return;
    }

    auto& transform = scene->GetComponent<TransformComponent>(entityId);

    transform.isDirty = transform.isDirty || isParentDirty;

    if (transform.isDirty) {
        transform.UpdateModelMatrix();

        if (parent->entityId != ecs::INVALID_ENTITY_ID) {
            auto parentTransform = scene->GetComponent<TransformComponent>(parent->entityId);
            transform.modelMatrix = transform.modelMatrix * parentTransform.modelMatrix;
        }
    }
    for (auto& child : children) {
        child->UpdateTransform(scene, transform.isDirty);
    }
    transform.isDirty = false;
}

void Scene::SceneNode::AddChild(SharedPtr<SceneNode> child)
{
    if (child->parent) {
        child->parent->RemoveChild(child->entityId);
    }
    children.push_back(child);
}

void Scene::SceneNode::RemoveChild(ecs::Entity entity)
{
    auto it = std::find_if(children.begin(), children.end(), [entity](const auto& child) {
        return child->entityId == entity;
    });

    if (it != children.end())
        children.erase(it);
}

} // namespace elv
