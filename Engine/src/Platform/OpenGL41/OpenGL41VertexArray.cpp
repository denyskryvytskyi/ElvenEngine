#include "OpenGL41VertexArray.h"

#include "Platform/OpenGL41/OpenGL41Check.h"

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

OpenGL41VertexArray::OpenGL41VertexArray()
{
    glCheck(
        glGenVertexArrays(1, &m_id));
}

OpenGL41VertexArray::~OpenGL41VertexArray()
{
    glCheck(glDeleteVertexArrays(1, &m_id));
}

void OpenGL41VertexArray::AddVertexBuffer(const SharedPtr<VertexBuffer>& vertexBuffer)
{
    Bind();

    const VertexBufferLayout& layout = vertexBuffer->GetLayout();
    glCheck(
        glBindBuffer(
            GL_ARRAY_BUFFER, vertexBuffer->GetId()));

    if (m_indexBuffer) {
        glCheck(
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBuffer->GetId()));
    }

    unsigned int attributeIndex = 0;
    for (const auto& attribute : layout.GetAttributes()) {
        glCheck(
            glVertexAttribPointer(
                attributeIndex,
                attribute.m_size,
                BufferAttributeTypeToOpenGLBaseType(attribute.m_type),
                attribute.m_normalized ? GL_TRUE : GL_FALSE,
                layout.GetStride(),
                (const void*)attribute.m_offset));

        glCheck(
            glEnableVertexAttribArray(attributeIndex));

        attributeIndex++;
    }
    m_vertexBufferIndex++;
    m_vertexBuffers.push_back(vertexBuffer);

    Unbind();
}

void OpenGL41VertexArray::SetIndexBuffer(const SharedPtr<IndexBuffer>& indexBuffer_)
{
    m_indexBuffer = indexBuffer_;
}

void OpenGL41VertexArray::Bind() const
{
    glCheck(glBindVertexArray(m_id));
}

void OpenGL41VertexArray::Unbind() const
{
    glCheck(glBindVertexArray(0));
}
} // namespace elv
