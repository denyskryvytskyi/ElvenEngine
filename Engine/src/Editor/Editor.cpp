#include "Editor.h"

#include "Editor/EditorHelpers.h"

#include "Core/Application.h"
#include "Core/SettingsConfig.h"
#include "Core/Window.h"
#include "Renderer/Renderer.h"

#include <imgui.h>

namespace elv::editor {
constexpr std::uint16_t kMaxMaskthreshold = 128;
constexpr std::uint16_t kMaskPreviewWidth = 128;
constexpr std::uint16_t kMaskPreviewHeight = 96;

void DrawPostProcessSettings(Renderer& renderer);

void Editor::OnInit()
{
    if (gEngineSettings.enableSceneGraph) {
        m_sceneHierarchyPanel.OnInit();
    } else {
        EL_CORE_WARN("Failed to init scene hierarchy panel: scene graph is turned off.");
    }
}

void Editor::OnImGuiRender()
{
    // Demo window
    if (false) {
        static bool showDemo = true;
        ImGui::ShowDemoWindow(&showDemo);
    }

    auto& window = Application::Get().GetWindow();
    auto& renderer = Application::Get().GetRenderer();

    bool isVSync = window->IsVSync();
    bool isFullScreen = window->IsFullScreen();
    bool isMSAAEnabled = renderer.IsMSAAEnabled();
    bool isBlinnPhongEnabled = renderer.IsBlinnPhongEnabled();

    // ============ Settings panel ============
    ImGui::Begin("Settings");
    ImGui::SetWindowSize(ImVec2(300.0f, 200.0f));

    if (ImGui::Checkbox("V-Sync", &isVSync)) {
        window->SetVSync(isVSync);
    }

    if (ImGui::Checkbox("Fullsreen mode", &isFullScreen)) {
        window->SetFullScreen(isFullScreen);
    }

    if (ImGui::Checkbox("MSAA", &isMSAAEnabled)) {
        renderer.EnableMSAA(isMSAAEnabled);
    }

    if (ImGui::Checkbox("Blinn-Phong", &isBlinnPhongEnabled)) {
        renderer.EnableBlinnPhong(isBlinnPhongEnabled);
    }

    DrawPostProcessSettings(renderer);

    ImGui::End();

    // ============ Environment ============
    ImGui::Begin("Environment");
    lia::vec4 clearColor = renderer.GetClearColor();
    if (DrawRGBAColorControl("Clear color", clearColor)) {
        renderer.SetClearColor(clearColor);
    }
    ImGui::End();

    // ============ Scene ============
    if (gEngineSettings.enableSceneGraph) {
        m_sceneHierarchyPanel.OnImGuiRender();
    }

    m_profileTelemetry.OnImGuiRender();
}

void DrawPostProcessSettings(Renderer& renderer)
{
    const ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_DefaultOpen
        | ImGuiTreeNodeFlags_Framed
        | ImGuiTreeNodeFlags_SpanAvailWidth
        | ImGuiTreeNodeFlags_AllowItemOverlap
        | ImGuiTreeNodeFlags_FramePadding;

    const bool opened = ImGui::TreeNodeEx("Post processing", flags);

    if (opened) {
        auto& postProcessor = renderer.GetPostProcessor();

        ImGui::Checkbox("Blur enabled", &postProcessor.IsBlurEnabled);
        DrawSliderInt("Threshold by width", 0, kMaxMaskthreshold, postProcessor.BlurMaskThreshold);

        const char* items[] = { "Linear", "Checkerboard", "Triangular", "Radial", "Exponential" };
        static int currentIndex = 0;
        const char* itemStr = items[currentIndex];
        if (ImGui::BeginCombo("Type", itemStr)) {
            for (int i = 0; i < IM_ARRAYSIZE(items); ++i) {
                const bool is_selected = (currentIndex == i);
                if (ImGui::Selectable(items[i], is_selected)) {
                    currentIndex = i;
                    itemStr = items[currentIndex];
                    postProcessor.GradientMaskType = static_cast<PostProcessor::BlurGradientMaskType>(currentIndex);
                }

                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        if (ImGui::Button("Rebuild mask")) {
            postProcessor.BuildBlurMask(postProcessor.GradientMaskType);
        }

        ImGui::Text("Mask preview:");
        ImVec2 uv_min = ImVec2(0.0f, 1.0f);
        ImVec2 uv_max = ImVec2(1.0f, 0.0f);
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f);

        ImGui::Image((void*)(intptr_t)postProcessor.GetMaskTextureId(), ImVec2(kMaskPreviewWidth, kMaskPreviewHeight), uv_min, uv_max, tint_col, border_col);
        ImGui::TreePop();
    }
}
} // namespace elv::editor
