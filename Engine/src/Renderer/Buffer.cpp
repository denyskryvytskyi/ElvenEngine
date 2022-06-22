#include "Renderer/Buffer.h"
#include "Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Elven {
SharedPtr<VertexBuffer> VertexBuffer::Create(std::uint32_t size)
{
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
        EL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return MakeSharedPtr<OpenGLVertexBuffer>(size);
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
    }

    EL_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
} // namespace Elven
