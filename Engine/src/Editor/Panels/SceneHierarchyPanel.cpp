#include "SceneHierarchyPanel.h"

#include "Editor/EditorHelpers.h"

#include "Scene/Components/LightComponent.h"
#include "Scene/Components/StaticMeshComponent.h"
#include "Scene/SceneManager.h"

#include <imgui.h>

namespace elv::editor {

template<typename ComponentType, typename UIFunction>
static void DrawComponent(const std::string& title,
                          const ecs::Entity entity,
                          Scene& scene,
                          UIFunction uiFunc)
{

    if (scene.HasComponent<ComponentType>(entity)) {
        const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen
            | ImGuiTreeNodeFlags_Framed
            | ImGuiTreeNodeFlags_SpanAvailWidth
            | ImGuiTreeNodeFlags_AllowItemOverlap
            | ImGuiTreeNodeFlags_FramePadding;

        const bool opened = ImGui::TreeNodeEx(title.c_str(), flags);

        if (opened) {
            auto& component = scene.GetComponent<ComponentType>(entity);

            uiFunc(component);

            ImGui::TreePop();
        }
    }
}

static bool HasChildren(const ecs::Entity entity, const std::vector<SceneNodeComponent>& nodes)
{
    for (const auto& node : nodes) {
        if (node.parent == entity) {
            return true;
        }
    }

    return false;
}

void SceneHierarchyPanel::OnInit()
{
    auto& scene = GetScene();
    m_nodesPool = scene.GetComponentPool<SceneNodeComponent>();
    m_tagsPool = scene.GetComponentPool<TagComponent>();

    if (!m_nodesPool || !m_tagsPool) {
        EL_CORE_CRITICAL("Failed to get one of the components pool");
    }
}

void SceneHierarchyPanel::OnImGuiRender()
{
    if (!m_nodesPool || !m_tagsPool) {
        return;
    }

    m_entityNameCounter = 0;
    auto& scene = GetScene();

    ImGui::Begin("Scene");
    DrawEntity(ecs::INVALID_ENTITY_ID, scene);
    ImGui::End();

    ImGui::Begin("Properties");
    DrawProperties(scene);
    ImGui::End();
}

void SceneHierarchyPanel::DrawEntity(const ecs::Entity parentEntity, Scene& scene)
{
    auto& nodes = m_nodesPool->GetComponents();

    for (std::uint32_t i = 0; i < nodes.size(); ++i) {

        SceneNodeComponent& nodeComponent = nodes[i];

        const bool isRootMode = parentEntity == ecs::INVALID_ENTITY_ID;

        // draw only root nodes
        if (isRootMode && nodeComponent.parent)
            continue;

        // search children
        if (!isRootMode && nodeComponent.parent != parentEntity) {
            continue;
        }

        const ecs::Entity nodeEntity = m_nodesPool->GetEntity(i);

        // skip itself
        if (nodeEntity == parentEntity)
            continue;

        const bool hasChildren = HasChildren(nodeEntity, nodes);

        // set flags
        ImGuiTreeNodeFlags flags = m_selectedEntity == nodeEntity ? ImGuiTreeNodeFlags_Selected : 0;
        if (hasChildren) {
            flags |= ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_SpanAvailWidth;
        } else {
            flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        }

        // draw entity
        std::string nodeName;
        if (scene.HasComponent<TagComponent>(nodeEntity)) {
            nodeName = m_tagsPool->GetComponent(nodeEntity).tag;
        } else {
            nodeName = fmt::format("Entity_{}", m_entityNameCounter++);
        }

        const bool opened = ImGui::TreeNodeEx((void*)(intptr_t)nodeEntity, flags, nodeName.c_str());
        if (ImGui::IsItemClicked()) {
            m_selectedEntity = nodeEntity;
        }

        if (opened && hasChildren) {
            DrawEntity(nodeEntity, scene);
            ImGui::TreePop();
        }
    }
}

void SceneHierarchyPanel::DrawProperties(Scene& scene)
{
    if (!m_selectedEntity)
        return;

    // ====== TAG ======
    {
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));

        if (scene.HasComponent<TagComponent>(m_selectedEntity)) {
            auto& tag = scene.GetComponent<TagComponent>(m_selectedEntity).tag;

            strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));

            if (ImGui::InputText("Tag", buffer, IM_ARRAYSIZE(buffer))) {
                tag = std::string(buffer);
            }
        } else {
            // add input and add tagComponent when value is set
            if (ImGui::InputText("Tag", buffer, IM_ARRAYSIZE(buffer))) {
                auto& tagComponent = scene.AddComponent<TagComponent>(m_selectedEntity);
                tagComponent.tag = buffer;
            }
        }
    }

    // ====== TRANSFORM ======
    DrawComponent<TransformComponent>("Transform", m_selectedEntity, scene, [](TransformComponent& component) {
        if (DrawVec3Control("model_pos", "Position", component.pos)) {
            component.isDirty = true;
        }

        if (DrawVec3Control("model_rotation", "Rotation", component.rotation)) {
            component.isDirty = true;
        }
        if (DrawVec3Control("model_scale", "Scale", component.scale)) {
            component.isDirty = true;
        }
    });

    // ====== STATIC MESH ======
    DrawComponent<StaticMeshComponent>("Static Mesh", m_selectedEntity, scene, [](StaticMeshComponent& component) {
        auto& material = component.GetMaterial();

        const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen
            | ImGuiTreeNodeFlags_Framed
            | ImGuiTreeNodeFlags_SpanAvailWidth
            | ImGuiTreeNodeFlags_AllowItemOverlap
            | ImGuiTreeNodeFlags_FramePadding;

        const bool opened = ImGui::TreeNodeEx("Material", flags);

        if (opened) {
            auto ambient = material.GetAmbientColor();
            auto diffuse = material.GetDiffuseColor();
            auto specular = material.GetSpecularColor();
            auto shininess = material.GetShininess();

            if (editor::DrawRGBColorControl("ambient", ambient)) {
                material.SetAmbientColor(ambient);
            }
            if (editor::DrawRGBColorControl("diffuse", diffuse)) {
                material.SetDiffuseColor(diffuse);
            }
            if (editor::DrawRGBColorControl("specular", specular)) {
                material.SetSpecularColor(specular);
            }
            if (editor::DrawSliderFloat("shininess", 1.0f, 256.0f, shininess)) {
                material.SetShininess(shininess);
            }

            ImGui::TreePop();
        }
    });

    // ====== LIGHT ======
    DrawComponent<DirectionalLightComponent>("Directional Light", m_selectedEntity, scene, [](DirectionalLightComponent& component) {
        ImGui::Checkbox("Enabled", &component.enabled);

        editor::DrawVec3Control("direction", "direction", component.direction);
        editor::DrawRGBColorControl("ambient", component.ambient);
        editor::DrawRGBColorControl("diffuse", component.diffuse);
        editor::DrawRGBColorControl("specular", component.specular);
    });

    DrawComponent<SpotLightComponent>("Spot Light", m_selectedEntity, scene, [](SpotLightComponent& component) {
        ImGui::Checkbox("Enabled", &component.enabled);

        editor::DrawSliderFloat("Cut off angle", 0.0f, 180.0f, component.cutOff);
        editor::DrawSliderFloat("Outer cut off angle", 0.0f, 180.0f, component.outerCutOff);

        editor::DrawRGBColorControl("ambient", component.ambient);
        editor::DrawRGBColorControl("diffuse", component.diffuse);
        editor::DrawRGBColorControl("specular", component.specular);
    });

    DrawComponent<PointLightComponent>("Point Light", m_selectedEntity, scene, [](PointLightComponent& component) {
        ImGui::Checkbox("Enabled", &component.enabled);

        editor::DrawRGBColorControl("ambient", component.ambient);
        editor::DrawRGBColorControl("diffuse", component.diffuse);
        editor::DrawRGBColorControl("specular", component.specular);
    });

    // ====== TEXT ======
    DrawComponent<TextComponent>("Text", m_selectedEntity, scene, [](TextComponent& component) {
        ImGui::Checkbox("Is visible##textVisible", &component.isVisible);
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strncpy_s(buffer, sizeof(buffer), component.text.c_str(), sizeof(buffer));

        if (ImGui::InputText("##text", buffer, IM_ARRAYSIZE(buffer))) {
            component.text = std::string(buffer);
        }

        editor::DrawRGBAColorControl("color##text", component.color);
    });

    DrawComponent<RectTransformComponent>("UI Transform", m_selectedEntity, scene, [](RectTransformComponent& component) {
        editor::DrawVec2Control("Position", "pos", component.pos);
        editor::DrawVec2Control("Scale", "scale", component.scale);
    });
}
} // namespace elv::editor
