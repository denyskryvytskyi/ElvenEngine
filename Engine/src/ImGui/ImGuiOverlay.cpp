#include <GLFW/glfw3.h>
#include <glad/gl.h>

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "ImGui/ImGuiOverlay.h"

#include "Core/Application.h"
#include "Core/Window.h"
#include "Renderer/Renderer2D.h"

namespace elv {

void ImGuiOverlay::Init()
{
    // setup Dear ImGui Context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;   // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable; // Enable Multi-Viewport / Platform Windows
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle& style = ImGui::GetStyle();
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        style.WindowRounding = 0.0f;
        style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    }

    // Setup dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsLight();

    // Setup Platform/Renderer bindings
    GLFWwindow* window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 330");

    m_isVSync = Application::Get().GetWindow()->IsVSync();
    m_isFullScreen = Application::Get().GetWindow()->IsFullScreen();
}

void ImGuiOverlay::Shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void ImGuiOverlay::ImGuiRender()
{
    static bool showDemo = true;
    ImGui::ShowDemoWindow(&showDemo);

    // ------- Custom panels -------
    ImGui::Begin("Menu");
    ImGui::SetWindowSize(ImVec2(300.0f, 200.0f));

    ImGui::Text("FPS: %.1f", Application::GetTelemetry().fps);
    ImGui::Text("Frame time %.3f ms", Application::GetTelemetry().frameTime);

    ImGui::Separator();

    ImGui::Text("2D renderer telemetry:");
    ImGui::Text("   draw calls: %i", Renderer2D::GetTelemetry().drawCalls);

    ImGui::Spacing();
    ImGui::Separator();

    ImGui::Text("Renderer: %s", glGetString(GL_RENDERER));

    ImGui::Separator();

    if (ImGui::Checkbox("V-Sync", &m_isVSync)) {
        Application::Get().GetWindow()->SetVSync(m_isVSync);
    }

    if (ImGui::Checkbox("Fullsreen mode", &m_isFullScreen)) {
        Application::Get().GetWindow()->SetFullScreen(m_isFullScreen);
    }

    ImGui::End();
}

void ImGuiOverlay::Begin()
{
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}

void ImGuiOverlay::End()
{
    ImGuiIO& io = ImGui::GetIO();
    Application& app = Application::Get();
    io.DisplaySize = ImVec2((float)app.GetWindow()->GetWidth(), (float)app.GetWindow()->GetHeight());

    // Rendering
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable) {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

} // namespace elv
