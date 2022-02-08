#pragma once

#include "Elven/Core/Core.h"
#include "Elven/Events/Event.h"

namespace Elven
{
    struct WindowProps
    {
        WindowProps(const std::string& title = "Elven Engine", unsigned int width = 1280, unsigned int height = 720)
            : Title(title), Width(width), Height(height)
        {
        }

    public:
        std::string Title;
        unsigned int Width = 0;
        unsigned int Height = 0;
    };

    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        virtual ~Window() = default;

        virtual void OnUpdate() = 0;

        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;

        // Window attributes
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual void* GetNativeWindow() const = 0;

        static Window* Create(const WindowProps& props = WindowProps());
    };
}