#pragma once

#include "Elven/Renderer/OrthographicCamera.h"
#include "Elven/Events/MouseEvent.h"
#include "Elven/Events/ApplicationEvent.h"

namespace Elven
{
    class OrthographicCameraController
    {
    public:
        OrthographicCameraController(float aspectRatio);
        ~OrthographicCameraController();

        void OnUpdate(float dt);
        void OnEvent(Events::Event& e);

        void OnResize(float width, float height);

        OrthographicCamera& GetCamera() { return m_Camera; }
        const OrthographicCamera& GetCamera() const { return m_Camera; }
        
        void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; }
        float GetZoomLevel() const { return m_ZoomLevel; }
        
    private:
        bool OnMouseScrolled(Events::MouseScrolledEvent& e);
        bool OnWindowResized(Events::WindowResizeEvent& e);

    private:
        OrthographicCamera m_Camera;

        gdm::vec3 m_Position;

        float m_AspectRatio;
        float m_ZoomLevel;
        float m_Rotation;
        float m_TranslationSpeed;
        float m_RotationSpeed;
    };
}