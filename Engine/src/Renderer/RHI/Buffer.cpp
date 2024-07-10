#include "Buffer.h"

#include "Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"
#include "Platform/OpenGL41/OpenGL41Buffer.h"

namespace elv {
SharedPtr<VertexBuffer> VertexBuffer::Create(std::uint32_t size)
{
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
        EL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return MakeSharedPtr<OpenGLVertexBuffer>(size);
    case RendererAPI::API::OpenGL41:
        return MakeSharedPtr<OpenGL41VertexBuffer>(size);
    }

    EL_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

SharedPtr<VertexBuffer> VertexBuffer::Create(float* vertices, std::uint32_t size)
{
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
        EL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return MakeSharedPtr<OpenGLVertexBuffer>(vertices, size);
    case RendererAPI::API::OpenGL41:
        return MakeSharedPtr<OpenGL41VertexBuffer>(vertices, size);
    }

    EL_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

///////////////////////////////////////////////////////

SharedPtr<IndexBuffer> IndexBuffer::Create(std::uint32_t* indices, std::uint32_t count)
{
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
        EL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return MakeSharedPtr<OpenGLIndexBuffer>(indices, count);
    case RendererAPI::API::OpenGL41:
        return MakeSharedPtr<OpenGL41IndexBuffer>(indices, count);
    }

    EL_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
} // namespace elv
