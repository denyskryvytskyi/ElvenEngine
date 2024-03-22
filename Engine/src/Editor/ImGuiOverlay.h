#pragma once

namespace elv {

class ImGuiOverlay {
public:
    ImGuiOverlay() = default;

    void Init();
    void Shutdown();

    void Begin();
    void End();
};

} // namespace elv
