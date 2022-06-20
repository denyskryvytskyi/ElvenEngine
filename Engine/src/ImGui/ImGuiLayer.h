#pragma once

#include "Core/Layer.h"

namespace Elven {

class ImGuiLayer : public Layer {
public:
    ImGuiLayer();

    void OnAttach() override;
    void OnDetach() override;
    void OnImGuiRender() override;

    void Begin();
    void End();

private:
    bool m_isVSync { false };
    bool m_isFullScreen { false };
};

} // namespace Elven
