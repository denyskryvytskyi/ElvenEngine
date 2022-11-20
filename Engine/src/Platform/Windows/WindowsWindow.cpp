#include "Platform/Windows/WindowsWindow.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyEvent.h"
#include "Events/MouseEvent.h"

#include "Events/EventManager.h"

#include <GLFW/glfw3.h>

namespace Elven {

static const unsigned int s_defaultWindowWidth = 1280;
static const unsigned int s_defaultWindowHeight = 720;
static const unsigned int s_defaultRefreshRate = 60;
static const unsigned int s_defaultWindowPosX = 100;
static const unsigned int s_defaultWindowPosY = 100;

static std::uint8_t s_GLFWwindowCount = 0;

static void GLFWErrorCallback(int error, const char* description)
{
    EL_CORE_ASSERT("GLFW Error ({0}): {1}", error, description);
}

WindowsWindow::WindowsWindow(const WindowProps& props)
{
    Init(props);
}

WindowsWindow::~WindowsWindow()
{
    Shutdown();
}

void WindowsWindow::OnUpdate()
{
    m_context->SwapBuffers();
    glfwPollEvents();
}

void WindowsWindow::SetVSync(bool enabled)
{
    if (enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

    m_data.VSync = enabled;
}

bool WindowsWindow::IsVSync() const
{
    return m_data.VSync;
}

void WindowsWindow::SetFullScreen(bool enabled)
{
    if (m_data.FullScreen != enabled) {
        m_data.FullScreen = enabled;

        if (enabled) {
            const GLFWvidmode* mode = glfwGetVideoMode(m_monitor);

            m_data.Width = mode->width;
            m_data.Height = mode->height;
            glfwSetWindowMonitor(m_window, m_monitor, 0, 0, m_data.Width, m_data.Height, mode->refreshRate);
        } else {
            m_data.Width = s_defaultWindowWidth;
            m_data.Height = s_defaultWindowHeight;
            glfwSetWindowMonitor(m_window, nullptr, 0, 0, m_data.Width, m_data.Height, s_defaultRefreshRate);
            glfwSetWindowPos(m_window, s_defaultWindowPosX, s_defaultWindowPosY);
        }

        UniquePtr<Events::WindowResizeEvent> resizeEvent = MakeUniquePtr<Events::WindowResizeEvent>(m_data.Width, m_data.Height);
        Events::QueueEvent(std::move(resizeEvent));
    }
}

bool WindowsWindow::IsFullScreen() const
{
    return m_data.FullScreen;
}

void WindowsWindow::Init(const WindowProps& props)
{
    m_data.Title = props.Title;
    m_data.Width = props.Width;
    m_data.Height = props.Height;
    m_data.VSync = props.IsVSync;
    m_data.FullScreen = props.IsFullScreen;

    EL_CORE_INFO("Creating window {} ({}, {})", props.Title, props.Width, props.Height);

    if (s_GLFWwindowCount == 0) {
        int success = glfwInit();
        EL_CORE_ASSERT(success, "Could not initialize GLFW!");
        glfwSetErrorCallback(GLFWErrorCallback);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);

    m_monitor = glfwGetPrimaryMonitor();
    if (m_data.FullScreen) {
        const GLFWvidmode* mode = glfwGetVideoMode(m_monitor);
        m_data.Width = mode->width;
        m_data.Height = mode->height;
    }
    m_window = glfwCreateWindow(m_data.Width, m_data.Height, m_data.Title.c_str(), m_data.FullScreen ? m_monitor : nullptr, nullptr);

    m_context = GraphicsContext::Create(m_window);
    m_context->Init();

    glfwSetWindowUserPointer(m_window, &m_data);
    SetVSync(m_data.VSync);

    // Set GLFW callbacks
    glfwSetWindowSizeCallback(m_window, [](GLFWwindow* window, int width, int height) {
        WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
        data.Width = width;
        data.Height = height;

        UniquePtr<Events::WindowResizeEvent> resizeEvent = MakeUniquePtr<Events::WindowResizeEvent>(width, height);
        Events::QueueEvent(std::move(resizeEvent));
    });

    glfwSetWindowCloseCallback(m_window, [](GLFWwindow* window) {
        UniquePtr<Events::WindowCloseEvent> closeEvent = MakeUniquePtr<Events::WindowCloseEvent>();
        Events::QueueEvent(std::move(closeEvent));
    });

    glfwSetKeyCallback(m_window, [](GLFWwindow* window, int key, int scancode, int action, int mode) {
        switch (action) {
        case GLFW_PRESS: {
            Events::TriggerEvent(Events::KeyPressedEvent(key, 0));
            break;
        }
        case GLFW_RELEASE: {
            Events::TriggerEvent(Events::KeyReleasedEvent(key));
            break;
        }
        case GLFW_REPEAT: {
            Events::TriggerEvent(Events::KeyPressedEvent(key, 1));
            break;
        }
        }
    });

    glfwSetCharCallback(m_window, [](GLFWwindow* window, unsigned int keycode) {
        UniquePtr<Events::KeyTypedEvent> keyTypeEvent = MakeUniquePtr<Events::KeyTypedEvent>(keycode);
        Events::QueueEvent(std::move(keyTypeEvent));
    });

    glfwSetMouseButtonCallback(m_window, [](GLFWwindow* window, int button, int action, int mods) {
        switch (action) {
        case GLFW_PRESS: {
            Events::TriggerEvent(Events::MouseButtonPressedEvent(button));
            break;
        }
        case GLFW_RELEASE: {
            Events::TriggerEvent(Events::MouseButtonReleasedEvent(button));
            break;
        }
        }
    });

    glfwSetScrollCallback(m_window, [](GLFWwindow* window, double xOffset, double yOffset) {
        Events::TriggerEvent(Events::MouseScrolledEvent((float)xOffset, (float)yOffset));
    });

    glfwSetCursorPosCallback(m_window, [](GLFWwindow* window, double xPos, double yPos) {
        Events::TriggerEvent(Events::MouseMovedEvent((float)xPos, (float)yPos));
    });
}

void WindowsWindow::Shutdown()
{
    glfwDestroyWindow(m_window);
    --s_GLFWwindowCount;

    if (s_GLFWwindowCount == 0) {
        glfwTerminate();
    }
}

} // namespace Elven
