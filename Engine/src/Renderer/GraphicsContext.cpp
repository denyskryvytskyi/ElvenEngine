#include "elpch.h"

#include "Renderer/GraphicsContext.h"
#include "Platform/OpenGL/OpenGLContext.h"

namespace Elven
{
    GraphicsContext* GraphicsContext::Create(void* window)
    {
        // Opengl maker only for now
        return new OpenGLContext(static_cast<GLFWwindow*>(window));
    }
}