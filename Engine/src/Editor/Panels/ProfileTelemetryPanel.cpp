#include "ProfileTelemetryPanel.h"

#include "Core/Application.h"
#include "Core/Profiler.h"
#include "Renderer/Renderer2D.h"

#include <imgui.h>
#include <imgui_internal.h>

namespace elv::editor {
void ProfileTelemetryPanel::OnImGuiRender()
{
    ImGui::Begin("Telemetry");

    const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen
        | ImGuiTreeNodeFlags_Framed
        | ImGuiTreeNodeFlags_SpanAvailWidth
        | ImGuiTreeNodeFlags_AllowItemOverlap
        | ImGuiTreeNodeFlags_FramePadding;

    {
        const bool opened = ImGui::TreeNodeEx("Performance", flags);

        if (opened) {

            ImGui::Text("FPS: %.1f", Application::GetTelemetry().fps);
            ImGui::Text("Frame time %.3f ms", Application::GetTelemetry().frameTime);

            ImGui::Separator();

            const auto& telemetry = ProfileTelemetry::GetInstance().GetTelemetry();

            for (const auto& info : telemetry) {
                ImGui::Text("%s: %.3f ms", info.first.c_str(), info.second);
            }

            ImGui::TreePop();
        }
    }

    {
        const bool opened = ImGui::TreeNodeEx("2D Renderer", flags);
        if (opened) {
            ImGui::Text("   draw calls: %i", Renderer2D::GetTelemetry().drawCalls);
            ImGui::TreePop();
        }
    }

    ImGui::End();
}
} // namespace elv::editor
