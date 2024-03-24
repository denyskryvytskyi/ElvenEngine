#pragma once

#include "Scene/Components/SceneComponents.h"
#include "Scene/Entity.h"
#include "Scene/Scene.h"
#include "Scene/SceneGraph.h"

namespace elv::editor {

class SceneHierarchyPanel {
public:
    void OnInit();
    void OnImGuiRender();

private:
    void DrawEntity(const ecs::Entity parentEntity);
    void DrawProperties();

    template<typename T>
    void DisplayAddComponentEntry(const std::string& name);

private:
    ecs::Entity m_selectedEntity { ecs::INVALID_ENTITY_ID };
    int m_entityNameCounter { 0 };

    Scene* m_context { nullptr };

    SharedPtr<ecs::ComponentPool<SceneNodeComponent>> m_nodesPool { nullptr };
    SharedPtr<ecs::ComponentPool<TagComponent>> m_tagsPool { nullptr };
};
} // namespace elv::editor
