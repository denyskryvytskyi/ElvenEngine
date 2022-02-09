#include "elpch.h"
#include "Platform/Windows/WindowsWindow.h"
#include "Platform/OpenGL/OpenGLContext.h"

#include "Elven/Events/ApplicationEvent.h"
#include "Elven/Events/KeyEvent.h"
#include "Elven/Events/MouseEvent.h"

#include "Elven/Events/EventManager.h"

namespace Elven
{
    static uint8_t s_GLFWindowCount = 0;

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
        glfwPollEvents();
        m_Context->SwapBuffers();
    }

    void WindowsWindow::SetVSync(bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.VSync = enabled;
    }

    bool WindowsWindow::IsVSync() const
    {
        return false;
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        EL_CORE_INFO("Creating window {0} ({1}, {2})", props.Title, props.Width, props.Height);

        if (s_GLFWindowCount == 0)
        {
            int success = glfwInit();
            EL_CORE_ASSERT(success, "Could not initialize GLFW!");
            glfwSetErrorCallback(GLFWErrorCallback);
        }

        m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

        m_Context = GraphicsContext::Create(m_Window);
        m_Context->Init();

        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        // Set GLFW callbacks
        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            gEventManager.Fire(new WindowResizeEvent(width, height));
            data.Width = width;
            data.Height = height;
        });

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            gEventManager.Fire(new WindowCloseEvent());
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mode)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
            case GLFW_PRESS:
            {
                gEventManager.Fire(new KeyPressedEvent(key, 0));
                break;
            }
            case GLFW_RELEASE:
            {
                gEventManager.Fire(new KeyReleasedEvent(key));
                break;
            }
            case GLFW_REPEAT:
            {
                gEventManager.Fire(new KeyPressedEvent(key, 1));
                break;
            }
            }
        });

        glfwSetCharCallback(m_Window, [](GLFWwindow* window, unsigned int keycode)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            gEventManager.Fire(new KeyTypedEvent(keycode));
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
            case GLFW_PRESS:
            {
                gEventManager.Fire(new MouseButtonPressedEvent(button));
                break;
            }
            case GLFW_RELEASE:
            {
                gEventManager.Fire(new MouseButtonReleasedEvent(button));
                break;
            }
            }
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            gEventManager.Fire(new MouseScrolledEvent((float)xOffset, (float)yOffset));
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float)xPos, (float)yPos);
            gEventManager.Fire(new MouseMovedEvent((float)xPos, (float)yPos));
        });
    }

    void WindowsWindow::Shutdown()
    {
        glfwDestroyWindow(m_Window);
        --s_GLFWindowCount;

        if (s_GLFWindowCount == 0)
        {
            glfwTerminate();
        }

        DeleteRawPointer(m_Context);
    }
}
