#include "elpch.h"
#include "OrthographicCameraController.h"

#include "Elven/Core/Input.h"
#include "Elven/Events/EventManager.h"
#include "Elven/Events/EventDispatcher.h"

namespace Elven
{
    OrthographicCameraController::OrthographicCameraController(float aspectRatio)
        : m_Camera(-aspectRatio, aspectRatio, -1.0f, 1.0f)
        , m_Position(0.0f), m_AspectRatio(aspectRatio), m_ZoomLevel(1.0f)
        , m_Rotation(0.0f), m_TranslationSpeed(5.0f), m_RotationSpeed(180.0f)
    {
        SUBSCRIBE_ON_EVENT(Events::MouseScrolledEvent::GetStaticUUID(), EL_BIND_EVENT_FN(OrthographicCameraController::OnEvent));
        SUBSCRIBE_ON_EVENT(Events::WindowResizeEvent::GetStaticUUID(), EL_BIND_EVENT_FN(OrthographicCameraController::OnEvent));
    }

    // need add timestep
    void OrthographicCameraController::OnUpdate(float dt)
    {
        EL_CORE_TRACE("Delta time {0} s", dt);

        if (Input::IsKeyPressed(Key::A))
        {
            m_Position.x -= cos(gdm::radians(m_Rotation)) * m_TranslationSpeed * dt;
            m_Position.y -= sin(gdm::radians(m_Rotation)) * m_TranslationSpeed * dt;
        }
        else if (Input::IsKeyPressed(Key::D))
        {
            m_Position.x += cos(gdm::radians(m_Rotation)) * m_TranslationSpeed * dt;
            m_Position.y += sin(gdm::radians(m_Rotation)) * m_TranslationSpeed * dt;
        }

        if (Input::IsKeyPressed(Key::W))
        {
            m_Position.x += -sin(gdm::radians(m_Rotation)) * m_TranslationSpeed * dt;
            m_Position.y += cos(gdm::radians(m_Rotation)) * m_TranslationSpeed * dt;
        }
        else if (Input::IsKeyPressed(Key::S))
        {
            m_Position.x -= -sin(gdm::radians(m_Rotation)) * m_TranslationSpeed * dt;
            m_Position.y -= cos(gdm::radians(m_Rotation)) * m_TranslationSpeed * dt;
        }

        if (Input::IsKeyPressed(Key::Q))
            m_Rotation += m_RotationSpeed * dt;
        if (Input::IsKeyPressed(Key::E))
            m_Rotation -= m_RotationSpeed * dt;

        if (m_Rotation > 180.0f)
            m_Rotation -= 360.0f;
        else if (m_Rotation <= -180.0f)
            m_Rotation += 360.0f;

        m_Camera.SetRotation(m_Rotation);
        m_Camera.SetPosition(m_Position);

        m_TranslationSpeed = m_ZoomLevel;
    }

    void OrthographicCameraController::OnEvent(Events::Event& e)
    {
        Events::EventDispatcher dispatcher(e);
        dispatcher.Dispatch<Events::MouseScrolledEvent>(EL_BIND_EVENT_FN(OrthographicCameraController::OnMouseScrolled));
        dispatcher.Dispatch<Events::WindowResizeEvent>(EL_BIND_EVENT_FN(OrthographicCameraController::OnWindowResized));
    }

    void OrthographicCameraController::OnResize(float width, float height)
    {
        m_AspectRatio = width / height;
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
    }

    bool OrthographicCameraController::OnMouseScrolled(Events::MouseScrolledEvent& e)
    {
        m_ZoomLevel -= e.GetYOffset() * 0.25f;
        m_ZoomLevel = std::max<float>(m_ZoomLevel, 0.25f);
        m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);
        return false;
    }

    bool OrthographicCameraController::OnWindowResized(Events::WindowResizeEvent& e)
    {
        OnResize(static_cast<float>(e.GetWidth()), static_cast<float>(e.GetHeight()));
        return false;
    }
}