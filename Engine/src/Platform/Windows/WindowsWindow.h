#pragma once

#include "Core/Window.h"
#include "Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Elven
{
    class WindowsWindow : public Window
    {
    private:
        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;
        };

    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        void OnUpdate() override;

        unsigned int GetWidth() const override { return m_Data.Width; }
        unsigned int GetHeight() const override { return m_Data.Height; }

        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        void* GetNativeWindow() const override { return m_Window; }

    private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();

    private:
        GLFWwindow* m_Window;
        GraphicsContext* m_Context;
        WindowData m_Data;
    };
}
