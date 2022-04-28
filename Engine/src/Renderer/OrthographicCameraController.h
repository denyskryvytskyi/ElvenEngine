#pragma once

#include "Renderer/OrthographicCamera.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"

namespace Elven
{
    class OrthographicCameraController
    {
    public:
        OrthographicCameraController(float aspectRatio);
        ~OrthographicCameraController();

        void OnUpdate(float dt);

        void OnResize(float width, float height);

        OrthographicCamera& GetCamera() { return m_Camera; }
        const OrthographicCamera& GetCamera() const { return m_Camera; }
        
        void SetZoomLevel(float zoomLevel) { m_ZoomLevel = zoomLevel; }
        float GetZoomLevel() const { return m_ZoomLevel; }
        
    private:
        bool OnMouseScrolled(Events::MouseScrolledEvent& e);
        bool OnWindowResized(Events::WindowResizeEvent& e);

    private:
        lia::vec3 m_Position;

        float m_AspectRatio = 0.0f;
        float m_ZoomLevel = 1.0f;
        float m_Rotation = 0.0f;
        float m_TranslationSpeed = 5.0f;
        float m_RotationSpeed = 180.0f;

        OrthographicCamera m_Camera;
    };
}