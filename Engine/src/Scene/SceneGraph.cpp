#include "SceneGraph.h"

#include "Core/Profiler.h"
#include "Scene/Scene.h"

namespace elv {
void SceneGraph::OnInit(Scene* scene)
{
    m_sceneNodesPool = scene->GetComponentPool<SceneNodeComponent>();
    m_transformsPool = scene->GetComponentPool<TransformComponent>();
}

void SceneGraph::OnUpdate(Scene* scene)
{
    // PROFILE("Scene graph updated in: ");
    /*
     * 1. Iterate all scene nodes and check if any parent is dirty
     * 2. Update worldMatrix if node or any parent is dirty
     * 3. Iterate transforms and scene nodes to reset dirty flag
     */
    auto& sceneNodeComponents = m_sceneNodesPool->GetComponents();

    // 1.
    {
        // PROFILE_SCOPE("\t1: ");
        for (uint32_t index = 0; index < sceneNodeComponents.size(); ++index) {
            auto& node = sceneNodeComponents.at(index);
            ecs::Entity entity = m_sceneNodesPool->GetEntity(index);

            if (!scene->HasComponent<TransformComponent>(entity)) {
                continue;
            }

            ecs::Entity parentId = node.parent;
            while (parentId != ecs::INVALID_ENTITY_ID) {
                if (!scene->HasComponent<TransformComponent>(parentId)) {
                    break;
                }
                const auto& parentTransform = m_transformsPool->GetComponent(parentId);
                if (parentTransform.isDirty) {
                    node.isParentDirty = true;
                    break;
                }
                const auto& parentNode = m_sceneNodesPool->GetComponent(parentId);
                parentId = parentNode.parent;
            }
        }
    }

    // 2.
    {
        // PROFILE_SCOPE("\t2: ");
        for (uint32_t index = 0; index < sceneNodeComponents.size(); ++index) {
            auto& node = sceneNodeComponents.at(index);

            ecs::Entity entity = m_sceneNodesPool->GetEntity(index);

            if (!scene->HasComponent<TransformComponent>(entity)) {
                continue;
            }
            auto& transform = m_transformsPool->GetComponent(entity);

            if (transform.isDirty || node.isParentDirty) {
                transform.UpdateLocalMatrix();
                transform.modelMatrix = transform.localMatrix;

                ecs::Entity parentId = node.parent;
                while (parentId != ecs::INVALID_ENTITY_ID) {
                    if (!scene->HasComponent<TransformComponent>(parentId)) {
                        break;
                    }
                    const auto& parentTransform = m_transformsPool->GetComponent(parentId);
                    transform.modelMatrix = transform.modelMatrix * parentTransform.localMatrix;

                    const auto& parentNode = m_sceneNodesPool->GetComponent(parentId);
                    parentId = parentNode.parent;
                }

                transform.normalMatrix = lia::transpose(lia::inverse(transform.modelMatrix));
            }
        }
    }

    // 3.
    {
        // PROFILE_SCOPE("\t3: ");
        for (auto& transform : m_transformsPool->GetComponents()) {
            transform.isDirty = false;
        }

        for (auto& transform : m_sceneNodesPool->GetComponents()) {
            transform.isParentDirty = false;
        }
    }
}
} // namespace elv
