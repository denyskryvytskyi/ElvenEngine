#include "VertexArray.h"

#include "Buffer.h"

#include "Renderer/Renderer.h"
#include "RendererAPI.h"

#include "Platform/OpenGL/OpenGLVertexArray.h"
#include "Platform/OpenGL41/OpenGL41VertexArray.h"

namespace elv {

SharedPtr<VertexArray> VertexArray::Create()
{
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
        EL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return MakeSharedPtr<OpenGLVertexArray>();
    case RendererAPI::API::OpenGL41:
        return MakeSharedPtr<OpenGL41VertexArray>();
    }

    EL_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

} // namespace elv
