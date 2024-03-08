#include "EditorHelpers.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace elv::editor {

namespace {
const float kResetValue = 0.0f;
const float kColumnWidth = 130.0f;
} // namespace

bool DrawVec3Control(const std::string& id, const std::string& label, lia::vec3& properties)
{
    bool changed = false;

    ImGuiIO& io = ImGui::GetIO();
    auto boldFont = io.Fonts->Fonts[0];

    ImGui::PushID(id.c_str());

    ImGui::Columns(2);
    ImGui::SetColumnWidth(0, kColumnWidth);
    ImGui::Text(label.c_str());
    ImGui::NextColumn();

    ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2 { 0, 0 });

    float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
    ImVec2 buttonSize = { lineHeight + 3.0f, lineHeight };

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 { 0.9f, 0.2f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 { 0.8f, 0.1f, 0.15f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("X", buttonSize))
        properties.x = kResetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    if (ImGui::DragFloat("##X", &properties.x, 0.1f, 0.0f, 0.0f, "%.2f")) {
        changed = true;
    }
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 { 0.3f, 0.8f, 0.3f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 { 0.2f, 0.7f, 0.2f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Y", buttonSize))
        properties.y = kResetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    if (ImGui::DragFloat("##Y", &properties.y, 0.1f, 0.0f, 0.0f, "%.2f")) {
        changed = true;
    }
    ImGui::PopItemWidth();
    ImGui::SameLine();

    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4 { 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4 { 0.2f, 0.35f, 0.9f, 1.0f });
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4 { 0.1f, 0.25f, 0.8f, 1.0f });
    ImGui::PushFont(boldFont);
    if (ImGui::Button("Z", buttonSize))
        properties.z = kResetValue;
    ImGui::PopFont();
    ImGui::PopStyleColor(3);

    ImGui::SameLine();
    if (ImGui::DragFloat("##Z", &properties.z, 0.1f, 0.0f, 0.0f, "%.2f")) {
        changed = true;
    }
    ImGui::PopItemWidth();

    ImGui::PopStyleVar();

    ImGui::Columns(1);

    ImGui::PopID();

    return changed;
}

bool DrawSliderFloat(const std::string& label, const float min, const float max, float& property)
{
    return ImGui::SliderFloat(label.c_str(), &property, min, max, "value = %.3f");
}

bool DrawRGBColorControl(const std::string& label, lia::vec3& colorProperty)
{
    return ImGui::ColorEdit3(label.c_str(), colorProperty.elementsPtr());
}

bool DrawRGBAColorControl(const std::string& label, lia::vec4& colorProperty)
{
    return ImGui::ColorEdit4(label.c_str(), colorProperty.elementsPtr());
}

} // namespace elv::editor
