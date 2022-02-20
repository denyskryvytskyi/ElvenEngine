#include "elpch.h"
#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <glad/glad.h>

namespace Elven
{
    static GLenum BufferAttributeTypeToOpenGLBaseType(BufferAttributeType type)
    {
        switch (type)
        {
        case BufferAttributeType::Float: return GL_FLOAT;
        case BufferAttributeType::Float2: return GL_FLOAT;
        case BufferAttributeType::Float3: return GL_FLOAT;
        case BufferAttributeType::Float4: return GL_FLOAT;
        case BufferAttributeType::Int: return GL_INT;
        case BufferAttributeType::Int2: return GL_INT;
        case BufferAttributeType::Int3: return GL_INT;
        case BufferAttributeType::Int4: return GL_INT;
        case BufferAttributeType::Bool: return GL_BOOL;
        }

        EL_CORE_ASSERT(false, "Unknown BufferAttributeType!");
        return 0;
    }

    ////////////////////////////////////////////////////////////////////////

    OpenGLVertexArray::OpenGLVertexArray()
    {
        glCreateVertexArrays(1, &m_RendererId);
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_RendererId);

        SafePointerDelete(m_IndexBuffer);
        for (size_t i = 0; i < m_VertexBuffers.size(); i++)
        {
            SafePointerDelete(m_VertexBuffers[i]);
        }
    }

    void OpenGLVertexArray::Bind() const
    {
        glBindVertexArray(m_RendererId);
    }

    void OpenGLVertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }

    void OpenGLVertexArray::AddVertexBuffer(VertexBuffer* vertexBuffer)
    {
        glBindVertexArray(m_RendererId);
        vertexBuffer->Bind();

        const VertexBufferLayout& layout = vertexBuffer->GetLayout();
        for (const auto& attribute : layout.GetAttributes())
        {
            glEnableVertexAttribArray(m_VertexBufferIndex);
            glVertexAttribPointer(m_VertexBufferIndex, attribute.GetComponentsCount(), BufferAttributeTypeToOpenGLBaseType(attribute.Type),
                attribute.Normalized ? GL_TRUE : GL_FALSE, layout.GetStride(), (const void*)attribute.Offset);
            m_VertexBufferIndex++;
        }

        m_VertexBuffers.push_back(vertexBuffer);
    }

    void OpenGLVertexArray::SetIndexBuffer(IndexBuffer* indexBuffer)
    {
        glBindVertexArray(m_RendererId);
        indexBuffer->Bind();

        m_IndexBuffer = indexBuffer;
    }
}