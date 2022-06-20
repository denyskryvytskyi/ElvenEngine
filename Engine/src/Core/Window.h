#pragma once

namespace Elven {

struct WindowProps {
    WindowProps(const std::string& title = "Elven Engine", unsigned int width = 1280, unsigned int height = 720, bool isFullScreen = false, bool isVSync = false)
        : Title(title)
        , Width(width)
        , Height(height)
        , IsFullScreen(isFullScreen)
        , IsVSync(isVSync)
    {
    }

public:
    std::string Title;
    unsigned int Width { 0 };
    unsigned int Height { 0 };
    bool IsFullScreen { false };
    bool IsVSync { false };
};

class Window {
public:
    virtual ~Window() = default;

    virtual void OnUpdate() = 0;

    virtual unsigned int GetWidth() const = 0;
    virtual unsigned int GetHeight() const = 0;

    // Window attributes
    virtual void SetVSync(bool enabled) = 0;
    virtual bool IsVSync() const = 0;

    virtual void SetFullScreen(bool enabled) = 0;
    virtual bool IsFullScreen() const = 0;

    virtual void* GetNativeWindow() const = 0;

    static Window* Create(const WindowProps& props = WindowProps());
};

} // namespace Elven
