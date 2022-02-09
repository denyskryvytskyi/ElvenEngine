#pragma once

#include "Elven/Core/Core.h"

#include <string>

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
        virtual ~Window() = default;

        virtual void OnUpdate() = 0;

        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;

        // Window attributes
        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual void* GetNativeWindow() const = 0;

        static Window* Create(const WindowProps& props = WindowProps());
    };
}