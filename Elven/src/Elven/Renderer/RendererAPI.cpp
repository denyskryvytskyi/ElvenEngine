#include "elpch.h"
#include "Elven/Renderer/RendererAPI.h"
#include "Platform/OpenGL/OpenGLRendererAPI.h"

namespace Elven
{
    RendererAPI::API RendererAPI::s_API = RendererAPI::API::OpenGL;

    RendererAPI* RendererAPI::Create()
    {
        switch (s_API)
        {
        case RendererAPI::API::None:
            EL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:
            return new OpenGLRendererAPI();
        }

        EL_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
