#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace elv {
RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

UniquePtr<RendererAPI> RendererAPI::Create()
{
    switch (s_API) {
    case RendererAPI::API::None:
        EL_CORE_ASSERT(false, "RendererAPI::None is currently not supported.");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return MakeUniquePtr<OpenGLRendererAPI>();
    }

    EL_CORE_ASSERT(false, "Unknown RendererAPI.");
    return nullptr;
}
} // namespace elv
