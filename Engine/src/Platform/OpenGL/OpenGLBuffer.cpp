#include "Platform/OpenGL/OpenGLBuffer.h"
#include <glad/gl.h>

namespace Elven {
OpenGLVertexBuffer::OpenGLVertexBuffer(std::uint32_t size)
{
    glCreateBuffers(1, &m_id);
    glNamedBufferStorage(m_id, size, nullptr, GL_DYNAMIC_STORAGE_BIT);
}

OpenGLVertexBuffer::OpenGLVertexBuffer(float* data, std::uint32_t size)
{
    glCreateBuffers(1, &m_id);
    glNamedBufferStorage(m_id, size, data, GL_DYNAMIC_STORAGE_BIT);
}

OpenGLVertexBuffer::~OpenGLVertexBuffer()
{
    glDeleteBuffers(1, &m_id);
}

void OpenGLVertexBuffer::SetData(const void* data, std::uint32_t size)
{
    glNamedBufferSubData(m_id, 0, size, data);
}

////////////////////////////////////////////////////

OpenGLIndexBuffer::OpenGLIndexBuffer(std::uint32_t* indices, std::uint32_t count)
    : m_count(count)
{
    glCreateBuffers(1, &m_id);
    glNamedBufferStorage(m_id, count * sizeof(std::uint32_t), indices, GL_DYNAMIC_STORAGE_BIT);
}

OpenGLIndexBuffer::~OpenGLIndexBuffer()
{
    glDeleteBuffers(1, &m_id);
}
} // namespace Elven
