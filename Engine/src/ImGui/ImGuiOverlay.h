#pragma once

namespace Elven {

class ImGuiOverlay {
public:
    ImGuiOverlay() = default;

    void Init();
    void Shutdown();
    void ImGuiRender();

    void Begin();
    void End();

private:
    bool m_isVSync { false };
    bool m_isFullScreen { false };
};

} // namespace Elven
