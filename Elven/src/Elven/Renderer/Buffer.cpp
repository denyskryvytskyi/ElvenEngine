#include "elpch.h"

#include "Elven/Renderer/Buffer.h"
#include "Elven/Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLBuffer.h"

namespace Elven
{
    VertexBufferAttribute::VertexBufferAttribute(BufferAttributeType type, const std::string& name, bool normalized)
        : Name(name)
        , Offset(0)
        , Size(BufferAttributeTypeSize(type))
        , Type(type)
        , Normalized(normalized)
    {
    }

    ///////////////////////////////////////////////////////

    VertexBufferLayout::VertexBufferLayout(const std::initializer_list<VertexBufferAttribute>& attributes)
        : m_Attributes(attributes)
        , m_Stride(0)
    {
        CalculateOffsetsAndStride();
    }

    ///////////////////////////////////////////////////////

    Ref<VertexBuffer> VertexBuffer::Create(uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:    EL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(size);
        }

        EL_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    Ref<VertexBuffer> VertexBuffer::Create(float* vertices, uint32_t size)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:    EL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return CreateRef<OpenGLVertexBuffer>(vertices, size);
        }

        EL_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }

    ///////////////////////////////////////////////////////

    Ref<IndexBuffer> IndexBuffer::Create(uint32_t* indices, uint32_t count)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::None:    EL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!"); return nullptr;
        case RendererAPI::API::OpenGL:  return CreateRef<OpenGLIndexBuffer>(indices, count);
        }

        EL_CORE_ASSERT(false, "Unknown RendererAPI!");
        return nullptr;
    }
}
