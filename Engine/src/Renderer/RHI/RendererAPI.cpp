#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLRendererAPI.h"
#include "Platform/OpenGL41/OpenGL41RendererAPI.h"

namespace elv {
// TODO: change to either runtime check or use more generic factor for checking
#ifdef __APPLE__
RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL41;
#else
RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;
#endif

UniquePtr<RendererAPI> RendererAPI::Create()
{
    switch (s_API) {
    case RendererAPI::API::None:
        EL_CORE_ASSERT(false, "RendererAPI::None is currently not supported.");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return MakeUniquePtr<OpenGLRendererAPI>();
    case RendererAPI::API::OpenGL41:
        return MakeUniquePtr<OpenGL41RendererAPI>();
    }

    EL_CORE_ASSERT(false, "Unknown RendererAPI.");
    return nullptr;
}
} // namespace elv
