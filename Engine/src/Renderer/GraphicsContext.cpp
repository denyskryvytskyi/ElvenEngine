#include "Renderer/GraphicsContext.h"
#include "Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLContext.h"

namespace elv {

UniquePtr<GraphicsContext> GraphicsContext::Create(void* window)
{
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
        EL_CORE_ASSERT(false, "RendererAPI::None is currently not supported.");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return MakeUniquePtr<OpenGLContext>(static_cast<GLFWwindow*>(window));
    }

    EL_CORE_ASSERT(false, "Unknown RendererAPI.");
    return nullptr;
}

} // namespace elv
