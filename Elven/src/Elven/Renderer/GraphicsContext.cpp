#include "elpch.h"

#include "Elven/Renderer/GraphicsContext.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Elven
{
    Scope<GraphicsContext> GraphicsContext::Create(void* window)
    {
        // todo: temporary opengl maker only for now
        return CreateScope<OpenGLContext>(static_cast<GLFWwindow*>(window));
    }
}