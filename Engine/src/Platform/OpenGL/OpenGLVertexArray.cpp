
#include "Platform/OpenGL/OpenGLVertexArray.h"

#include <glad/gl.h>

namespace elv {
static GLenum BufferAttributeTypeToOpenGLBaseType(BufferAttributeType type)
{
    switch (type) {
    case BufferAttributeType::Float:
        return GL_FLOAT;
    case BufferAttributeType::Float2:
        return GL_FLOAT;
    case BufferAttributeType::Float3:
        return GL_FLOAT;
    case BufferAttributeType::Float4:
        return GL_FLOAT;
    case BufferAttributeType::Int:
        return GL_INT;
    case BufferAttributeType::Int2:
        return GL_INT;
    case BufferAttributeType::Int3:
        return GL_INT;
    case BufferAttributeType::Int4:
        return GL_INT;
    case BufferAttributeType::Bool:
        return GL_BOOL;
    }

    EL_CORE_ASSERT(false, "Unknown BufferAttributeType!");
    return 0;
}

////////////////////////////////////////////////////////////////////////

OpenGLVertexArray::OpenGLVertexArray()
{
    glCreateVertexArrays(1, &m_id);
}

OpenGLVertexArray::~OpenGLVertexArray()
{
    glDeleteVertexArrays(1, &m_id);
}

void OpenGLVertexArray::AddVertexBuffer(const SharedPtr<VertexBuffer>& vertexBuffer)
{
    const VertexBufferLayout& layout = vertexBuffer->GetLayout();
    glVertexArrayVertexBuffer(m_id, m_vertexBufferIndex, vertexBuffer->GetId(), 0, layout.GetStride());

    unsigned int attributeIndex = 0;
    for (const auto& attribute : layout.GetAttributes()) {
        glEnableVertexArrayAttrib(m_id, attributeIndex);
        glVertexArrayAttribFormat(m_id, attributeIndex, attribute.GetComponentsCount(), BufferAttributeTypeToOpenGLBaseType(attribute.m_type),
                                  attribute.m_normalized ? GL_TRUE : GL_FALSE, (GLuint)attribute.m_offset);
        glVertexArrayAttribBinding(m_id, attributeIndex, m_vertexBufferIndex);

        attributeIndex++;
    }
    m_vertexBufferIndex++;
    m_vertexBuffers.push_back(vertexBuffer);
}

void OpenGLVertexArray::SetIndexBuffer(const SharedPtr<IndexBuffer>& indexBuffer_)
{
    glVertexArrayElementBuffer(m_id, indexBuffer_->GetId());
    m_indexBuffer = indexBuffer_;
}

void OpenGLVertexArray::Bind() const
{
    glBindVertexArray(m_id);
}
} // namespace elv
