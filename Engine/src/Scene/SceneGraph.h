#pragma once

#include "Scene/Component.h"
#include "Scene/Components/TransformComponent.h"

namespace elv {

class Scene;

struct SceneNodeComponent {
    SceneNodeComponent() = default;
    SceneNodeComponent(const ecs::Entity parentEntity)
        : parent(parentEntity)
    { }

    ecs::Entity parent { ecs::INVALID_ENTITY_ID };
    bool isParentDirty { false };
};

class SceneGraph {
public:
    void OnInit(Scene* scene);
    void OnUpdate(Scene* scene);

private:
    SharedPtr<ecs::ComponentPool<SceneNodeComponent>> m_sceneNodesPool { nullptr };
    SharedPtr<ecs::ComponentPool<TransformComponent>> m_transformsPool { nullptr };
};

} // namespace elv
