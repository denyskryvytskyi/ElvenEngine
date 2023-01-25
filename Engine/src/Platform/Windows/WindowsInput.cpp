#include "Core/Application.h"
#include "Core/Input.h"
#include "Core/Window.h"
#include <GLFW/glfw3.h>

namespace elv {

bool Input::IsKeyPressed(Keycode key)
{
    auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
    auto state = glfwGetKey(window, key);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMousePressed(MouseCode button)
{
    auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
    auto state = glfwGetMouseButton(window, button);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

std::pair<float, float> Input::GetMousePosition()
{
    auto* window = static_cast<GLFWwindow*>(Application::Get().GetWindow()->GetNativeWindow());
    double xPos, yPos;
    glfwGetCursorPos(window, &xPos, &yPos);
    return std::make_pair((float)xPos, (float)yPos);
}

float Input::GetMouseX()
{
    return GetMousePosition().first;
}

float Input::GetMouseY()
{
    return GetMousePosition().second;
}

} // namespace elv
