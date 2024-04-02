#include "Editor.h"

#include "Editor/EditorHelpers.h"

#include "Core/Application.h"
#include "Core/SettingsConfig.h"
#include "Core/Window.h"
#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"

#include <imgui.h>

namespace elv::editor {
void Editor::OnInit()
{
    m_isVSync = Application::Get().GetWindow()->IsVSync();
    m_isFullScreen = Application::Get().GetWindow()->IsFullScreen();

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

    // ============ Settings panel ============
    ImGui::Begin("Settings");
    ImGui::SetWindowSize(ImVec2(300.0f, 200.0f));

    if (ImGui::Checkbox("V-Sync", &m_isVSync)) {
        Application::Get().GetWindow()->SetVSync(m_isVSync);
    }

    if (ImGui::Checkbox("Fullsreen mode", &m_isFullScreen)) {
        Application::Get().GetWindow()->SetFullScreen(m_isFullScreen);
    }

    if (ImGui::Checkbox("MSAA", &m_isMSAAEnabled)) {
        RenderCommand::EnableMSAA(m_isMSAAEnabled);
    }
    ImGui::End();

    // ============ Environment ============
    ImGui::Begin("Environment");
    auto clearColor = Renderer::GetClearColor();
    if (DrawRGBAColorControl("Clear color", clearColor)) {
        Renderer::SetClearColor(clearColor);
    }
    ImGui::End();

    // ============ Scene ============
    if (gEngineSettings.enableSceneGraph) {
        m_sceneHierarchyPanel.OnImGuiRender();
    }

    m_profileTelemetry.OnImGuiRender();
}
} // namespace elv::editor
