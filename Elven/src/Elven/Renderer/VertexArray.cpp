#include "elpch.h"
#include "Elven/Renderer/VertexArray.h"
#include "Elven/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"

namespace Elven
{
    Ref<VertexArray> VertexArray::Create()
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:    EL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexArray>();
        }

        EL_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
