#include "SceneHierarchyPanel.h"

#include "Editor/EditorHelpers.h"

#include "Scene/Components/LightComponent.h"
#include "Scene/Components/StaticMeshComponent.h"
#include "Scene/SceneManager.h"

#include "Resources/AudioManager.h"
#include "Resources/MeshLibrary.h"
#include "Resources/TextureManager.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace elv::editor {

const char* kMaterialMaps[] = { "Diffuse map",
                                "Specular map",
                                "Normal map",
                                "Emission map",
                                "Transparency map" };

template<typename ComponentType, typename UIFunction>
static void DrawComponent(const std::string& title,
                          const ecs::Entity entity,
                          Scene* scene,
                          UIFunction uiFunc)
{

    if (scene->HasComponent<ComponentType>(entity)) {
        const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen
            | ImGuiTreeNodeFlags_Framed
            | ImGuiTreeNodeFlags_SpanAvailWidth
            | ImGuiTreeNodeFlags_AllowItemOverlap
            | ImGuiTreeNodeFlags_FramePadding;

        ImVec2 contentRegionAvailable = ImGui::GetContentRegionAvail();
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2 { 4, 4 });

        const bool opened = ImGui::TreeNodeEx(title.c_str(), flags);
        ImGui::PopStyleVar();

        const float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
        ImGui::SameLine(contentRegionAvailable.x - lineHeight * 0.5f);
        if (ImGui::Button("...", ImVec2 { lineHeight, lineHeight })) {
            ImGui::OpenPopup("ComponentSettings");
        }

        bool removeComponent = false;
        if (ImGui::BeginPopup("ComponentSettings")) {
            if (ImGui::MenuItem("Remove component"))
                removeComponent = true;

            ImGui::EndPopup();
        }

        if (opened) {
            auto& component = scene->GetComponent<ComponentType>(entity);

            uiFunc(component);

            ImGui::TreePop();
        }

        if (removeComponent) {
            scene->RemoveComponent<ComponentType>(entity);
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
    m_context = &GetScene();

    m_nodesPool = m_context->GetComponentPool<SceneNodeComponent>();
    m_tagsPool = m_context->GetComponentPool<TagComponent>();

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

    ImGui::Begin("Scene");
    DrawEntity(ecs::INVALID_ENTITY_ID);

    // Context menu
    if (ImGui::BeginPopupContextWindow(0, 1, false)) {
        if (ImGui::MenuItem("Create Entity"))
            m_context->CreateEntity();

        ImGui::EndPopup();
    }
    ImGui::End();

    ImGui::Begin("Properties");
    DrawProperties();
    ImGui::End();
}

void SceneHierarchyPanel::DrawEntity(const ecs::Entity parentEntity)
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
        if (m_context->HasComponent<TagComponent>(nodeEntity)) {
            nodeName = m_tagsPool->GetComponent(nodeEntity).tag;
        } else {
            nodeName = fmt::format("Entity_{}", m_entityNameCounter++);
        }

        const bool opened = ImGui::TreeNodeEx((void*)(intptr_t)nodeEntity, flags, nodeName.c_str());
        if (ImGui::IsItemClicked()) {
            m_selectedEntity = nodeEntity;
        }

        // Context menu
        bool isEntityToDelete = false;
        if (ImGui::BeginPopupContextItem(fmt::format("Entity menu{}", nodeEntity).c_str())) {
            if (ImGui::MenuItem("Create Child Entity"))
                m_context->CreateChildEntity(nodeEntity);
            if (ImGui::MenuItem("Delete Entity"))
                isEntityToDelete = true;
            ImGui::EndPopup();
        }

        if (opened && hasChildren) {
            DrawEntity(nodeEntity);
            ImGui::TreePop();
        }

        if (isEntityToDelete) {
            // make it always invalid because we can destroy child entity that was selected
            m_selectedEntity = ecs::INVALID_ENTITY_ID;

            m_context->DestroyEntity(nodeEntity);
        }
    }
}

void SceneHierarchyPanel::DrawProperties()
{
    if (!m_selectedEntity)
        return;

    ImGui::PushItemWidth(-1);

    if (ImGui::Button("Add Component"))
        ImGui::OpenPopup("AddComponent");

    if (ImGui::BeginPopup("AddComponent")) {
        DisplayAddComponentEntry<TransformComponent>("Transform");
        DisplayAddComponentEntry<StaticMeshComponent>("Static Mesh");
        DisplayAddComponentEntry<DirectionalLightComponent>("Directional Light");
        DisplayAddComponentEntry<PointLightComponent>("Point Light");
        DisplayAddComponentEntry<SpotLightComponent>("Spotlight");
        DisplayAddComponentEntry<TextComponent>("Text");
        DisplayAddComponentEntry<RectTransformComponent>("UI Transform");
        DisplayAddComponentEntry<SpriteComponent>("Sprite");
        DisplayAddComponentEntry<SoundComponent>("Sound");

        ImGui::EndPopup();
    }

    ImGui::PopItemWidth();

    // ====== TAG ======
    {
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));

        if (m_context->HasComponent<TagComponent>(m_selectedEntity)) {
            auto& tag = m_context->GetComponent<TagComponent>(m_selectedEntity).tag;

            strncpy_s(buffer, sizeof(buffer), tag.c_str(), sizeof(buffer));

            if (ImGui::InputText("Tag", buffer, IM_ARRAYSIZE(buffer))) {
                tag = std::string(buffer);
            }
        } else {
            // add input and add tagComponent when value is set
            if (ImGui::InputText("Tag", buffer, IM_ARRAYSIZE(buffer))) {
                auto& tagComponent = m_context->AddComponent<TagComponent>(m_selectedEntity);
                tagComponent.tag = buffer;
            }
        }
    }

    // ====== TRANSFORM ======
    DrawComponent<TransformComponent>("Transform", m_selectedEntity, m_context, [](TransformComponent& component) {
        lia::vec3 position = component.GetPosition();
        if (DrawVec3Control("model_pos", "Position", position)) {
            component.SetPosition(position);
        }

        lia::vec3 rotation = lia::degrees(component.GetRotation());
        if (DrawVec3Control("model_rotation", "Rotation", rotation)) {
            component.SetRotation(lia::radians(rotation));
        }

        lia::vec3 scale = component.GetScale();
        if (DrawVec3Control("model_scale", "Scale", scale)) {
            component.SetScale(scale);
        }
    });

    // ====== STATIC MESH ======
    DrawComponent<StaticMeshComponent>("Static Mesh", m_selectedEntity, m_context, [](StaticMeshComponent& component) {
        // Mesh name
        const auto names = gMeshLibrary.GetMeshes();

        static std::uint64_t currentMeshIndex = 0;
        const bool isMeshSet = component.GetMeshPtr() != nullptr;
        std::string comboLabel;

        if (isMeshSet) {
            auto it = std::find(names.begin(), names.end(), component.GetMeshName());

            if (it != names.end()) {
                currentMeshIndex = std::distance(names.begin(), it);
                comboLabel = names[currentMeshIndex];
            }
        } else {
            comboLabel = "cube";
            auto it = std::find(names.begin(), names.end(), comboLabel);
            if (it != names.end()) {
                currentMeshIndex = std::distance(names.begin(), it);
            }
            component.ResetMesh(comboLabel);
            component.SetName(comboLabel);
        }

        if (ImGui::BeginCombo("Mesh", comboLabel.c_str())) {
            for (int i = 0; i < names.size(); ++i) {
                const bool is_selected = (currentMeshIndex == i);
                if (ImGui::Selectable(names[i].c_str(), is_selected)) {
                    currentMeshIndex = i;
                    comboLabel = names[currentMeshIndex];
                    component.ResetMesh(comboLabel);
                    component.SetName(comboLabel);
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        // Render Topology
        const char* items[] = { "Triangles", "Triangle strip", "Lines", "Lines strip" };
        static int currentTopologyIndex = 0;

        if (isMeshSet) {
            const RenderTopology currentTopology = component.GetRenderTopology();
            currentTopologyIndex = static_cast<int>(currentTopology);
        }

        const char* topologyStr = items[currentTopologyIndex];
        if (ImGui::BeginCombo("Render topology", topologyStr)) {
            for (int i = 0; i < IM_ARRAYSIZE(items); ++i) {
                const bool is_selected = (currentTopologyIndex == i);
                if (ImGui::Selectable(items[i], is_selected)) {
                    currentTopologyIndex = i;
                    topologyStr = items[currentTopologyIndex];
                    const RenderTopology topology = static_cast<RenderTopology>(currentTopologyIndex);
                    component.SetTopology(topology);
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        // Material
        if (Material* material = component.GetMaterial()) {
            const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen
                | ImGuiTreeNodeFlags_Framed
                | ImGuiTreeNodeFlags_SpanAvailWidth
                | ImGuiTreeNodeFlags_AllowItemOverlap
                | ImGuiTreeNodeFlags_FramePadding;

            const bool opened = ImGui::TreeNodeEx("Material", flags);

            if (opened) {
                lia::vec3 ambient = material->GetAmbientColor();
                lia::vec3 diffuse = material->GetDiffuseColor();
                lia::vec3 specular = material->GetSpecularColor();
                lia::vec3 emissive = material->GetEmissionColor();
                float shininess = material->GetShininess();

                if (editor::DrawRGBColorControl("ambient", ambient)) {
                    material->SetAmbientColor(ambient);
                }
                if (editor::DrawRGBColorControl("diffuse", diffuse)) {
                    material->SetDiffuseColor(diffuse);
                }
                if (editor::DrawRGBColorControl("specular", specular)) {
                    material->SetSpecularColor(specular);
                }
                if (editor::DrawRGBColorControl("emissive", emissive)) {
                    material->SetEmissionColor(emissive);
                }
                if (editor::DrawSliderFloat("shininess", 1.0f, 256.0f, shininess)) {
                    material->SetShininess(shininess);
                }

                // Textures
                if (Material* material = component.GetMaterial()) {
                    const auto textures = gTextureManager.GetTextureNames();
                    static std::uint64_t mapIndices[Material::TextureSlot::Count];

                    for (int i = 0; i < Material::TextureSlot::Count; ++i) {

                        const Material::TextureSlot slot = static_cast<Material::TextureSlot>(i);
                        const std::string currentMapName = material->GetTextureName(slot);

                        std::string texture;

                        if (!currentMapName.empty()) {
                            auto it = std::find(textures.begin(), textures.end(), currentMapName);
                            if (it != textures.end()) {
                                mapIndices[i] = std::distance(textures.begin(), it);
                            }

                            texture = textures[mapIndices[i]];
                        }

                        if (ImGui::BeginCombo(kMaterialMaps[i], texture.c_str())) {
                            for (int j = 0; j < textures.size(); ++j) {
                                const bool is_selected = (mapIndices[i] == j);
                                if (ImGui::Selectable(textures[j].c_str(), is_selected)) {
                                    mapIndices[i] = j;
                                    texture = textures[j];
                                    material->SetTexture(slot, texture, "", true);
                                }

                                if (is_selected)
                                    ImGui::SetItemDefaultFocus();
                            }
                            ImGui::EndCombo();
                        }
                    }
                }
                //

                ImGui::TreePop();
            }
        }
    });

    // ====== LIGHT ======
    DrawComponent<DirectionalLightComponent>("Directional Light", m_selectedEntity, m_context, [](DirectionalLightComponent& component) {
        ImGui::Checkbox("Enabled", &component.enabled);

        editor::DrawVec3Control("direction", "Direction", component.direction);
        ImGui::Spacing();
        editor::DrawRGBColorControl("ambient", component.ambient);
        editor::DrawRGBColorControl("diffuse", component.diffuse);
        editor::DrawRGBColorControl("specular", component.specular);
    });

    DrawComponent<SpotLightComponent>("Spot Light", m_selectedEntity, m_context, [](SpotLightComponent& component) {
        ImGui::Checkbox("Enabled", &component.enabled);
        ImGui::Checkbox("Flashlight mode", &component.flashlightMode);
        ImGui::Checkbox("Debug render", &component.debugRender);

        editor::DrawSliderFloat("Cut off angle", 0.0f, 180.0f, component.cutOff);
        editor::DrawSliderFloat("Outer cut off angle", 0.0f, 180.0f, component.outerCutOff);

        editor::DrawRGBColorControl("ambient", component.ambient);
        editor::DrawRGBColorControl("diffuse", component.diffuse);
        editor::DrawRGBColorControl("specular", component.specular);
    });

    DrawComponent<PointLightComponent>("Point Light", m_selectedEntity, m_context, [](PointLightComponent& component) {
        ImGui::Checkbox("Enabled", &component.enabled);
        ImGui::Checkbox("Debug render", &component.debugRender);

        editor::DrawRGBColorControl("ambient", component.ambient);
        editor::DrawRGBColorControl("diffuse", component.diffuse);
        editor::DrawRGBColorControl("specular", component.specular);
    });

    // ====== TEXT ======
    DrawComponent<TextComponent>("Text", m_selectedEntity, m_context, [](TextComponent& component) {
        ImGui::Checkbox("Is visible##textVisible", &component.isVisible);
        char buffer[256];
        memset(buffer, 0, sizeof(buffer));
        strncpy_s(buffer, sizeof(buffer), component.text.c_str(), sizeof(buffer));

        if (ImGui::InputText("##text", buffer, IM_ARRAYSIZE(buffer))) {
            component.text = std::string(buffer);
        }

        editor::DrawRGBAColorControl("color##text", component.color);
    });

    DrawComponent<RectTransformComponent>("UI Transform", m_selectedEntity, m_context, [](RectTransformComponent& component) {
        editor::DrawVec2Control("Position", "pos", component.pos);
        editor::DrawVec2Control("Scale", "scale", component.scale);
    });

    // ====== SPRITE ======
    DrawComponent<SpriteComponent>("Sprite", m_selectedEntity, m_context, [](SpriteComponent& component) {
        editor::DrawRGBAColorControl("color##sprite", component.color);
    });

    // ====== SOUND ======
    DrawComponent<SoundComponent>("Sound", m_selectedEntity, m_context, [](SoundComponent& component) {
        if (ImGui::Button("Play")) {
            component.Play();
        }

        ImGui::SameLine();
        if (ImGui::Button("Pause")) {
            component.Pause();
        }

        ImGui::SameLine();
        if (ImGui::Button("Stop")) {
            component.Stop();
        }

        ImGui::Checkbox("Loop", &component.isLooped);

        if (editor::DrawSliderFloat("Volume", 0.0f, 1.0f, component.volume)) {
            component.UpdateVolume();
        }

        const auto sounds = gAudioManager.GetSounds();
        static std::uint64_t currentSoundIndex = 0;

        if (!component.soundName.empty()) {
            auto it = std::find(sounds.begin(), sounds.end(), component.soundName);
            if (it != sounds.end()) {
                currentSoundIndex = std::distance(sounds.begin(), it);
            }
        }

        if (ImGui::BeginCombo("Sound", component.soundName.c_str())) {
            for (int i = 0; i < sounds.size(); ++i) {
                const bool is_selected = (currentSoundIndex == i);
                if (ImGui::Selectable(sounds[i].c_str(), is_selected)) {
                    currentSoundIndex = i;
                    component.soundName = sounds[currentSoundIndex];
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }
    });
}

template<typename ComponentType>
void SceneHierarchyPanel::DisplayAddComponentEntry(const std::string& name)
{
    if (!m_context->HasComponent<ComponentType>(m_selectedEntity)) {
        if (ImGui::MenuItem(name.c_str())) {
            m_context->AddComponent<ComponentType>(m_selectedEntity);
            ImGui::CloseCurrentPopup();
        }
    }
}
} // namespace elv::editor
