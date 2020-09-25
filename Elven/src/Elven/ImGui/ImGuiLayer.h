#pragma once

#include "Elven/Core/Layer.h"
#include "Elven/Events/ApplicationEvent.h"
#include "Elven/Events/KeyEvent.h"
#include "Elven/Events/MouseEvent.h"

namespace Elven
{
    class ELVEN_API ImGuiLayer : public Layer
    {
    public:
        ImGuiLayer();
        ~ImGuiLayer();

        void OnAttach();
        void OnDetach();
        void OnUpdate();
        void OnEvent(Event& event);

    private:
        bool OnMouseMovedEvent(MouseMovedEvent& e);
        bool OnMouseScrolledEvent(MouseScrolledEvent& e);
        bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& e);
        bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& e);
        bool OnKeyPressedEvent(KeyPressedEvent& e);
        bool OnKeyReleasedEvent(KeyReleasedEvent& e);
        bool OnKeyTypedEvent(KeyTypedEvent& e);
        bool OnWindowResizeEvent(WindowResizeEvent& e);

    private:
        float m_Time = 0.0f;
    };
}