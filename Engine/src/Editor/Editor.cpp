#include "Editor.h"

#include "Editor/EditorHelpers.h"

#include "Core/Application.h"
#include "Core/SettingsConfig.h"
#include "Core/Window.h"
#include "Renderer/Renderer.h"

#include <imgui.h>

namespace elv::editor {
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
    ImGui::End();

    // ============ Environment ============
    ImGui::Begin("Environment");
    auto clearColor = renderer.GetClearColor();
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
} // namespace elv::editor
