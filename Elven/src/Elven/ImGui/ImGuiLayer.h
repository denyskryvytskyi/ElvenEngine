#pragma once

#include "Elven/Core/Layer.h"
#include "Elven/Events/ApplicationEvent.h"
#include "Elven/Events/KeyEvent.h"
#include "Elven/Events/MouseEvent.h"

namespace Elven
{
    class ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer() = default;

        virtual void OnAttach() override;
        virtual void OnDetach() override;
        virtual void OnImGuiRender() override;

        void Begin();
        void End();

    private:
        float m_Time = 0.0f;
    };
}