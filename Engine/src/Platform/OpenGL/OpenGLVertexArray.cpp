#include "OpenGLVertexArray.h"

#include "Platform/OpenGLCommon.h"

#include <glad/gl.h>

namespace elv {

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
        glVertexArrayAttribFormat(m_id, attributeIndex, attribute.GetComponentsCount(), OpenGL::BufferAttributeTypeToOpenGLBaseType(attribute.m_type),
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
