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
    float m_Time { 0.0f };
    bool m_isVSync { false };
    bool m_isFullScreen { true };
};

} // namespace Elven
