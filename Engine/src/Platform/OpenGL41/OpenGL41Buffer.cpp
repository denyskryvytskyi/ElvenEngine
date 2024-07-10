#include "Platform/OpenGL41/OpenGL41Buffer.h"
#include "Platform/OpenGL41/OpenGL41Check.h"
#include <glad/gl.h>

namespace elv {
OpenGL41VertexBuffer::OpenGL41VertexBuffer(std::uint32_t size)
{
    glCheck(glGenBuffers(1, &m_id));
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_id));
    glCheck(glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW));
}

OpenGL41VertexBuffer::OpenGL41VertexBuffer(float* data, std::uint32_t size)
{
    glCheck(glGenBuffers(1, &m_id));
    glCheck(glBindBuffer(GL_ARRAY_BUFFER, m_id));
    glCheck(glBufferData(GL_ARRAY_BUFFER, size, data, GL_DYNAMIC_DRAW));
}

OpenGL41VertexBuffer::~OpenGL41VertexBuffer()
{
    glCheck(glDeleteBuffers(1, &m_id));
}

void OpenGL41VertexBuffer::SetData(const void* data, std::uint32_t size)
{
    glCheck(
        glBufferSubData(GL_ARRAY_BUFFER, 0, size, data));
}

////////////////////////////////////////////////////

OpenGL41IndexBuffer::OpenGL41IndexBuffer(std::uint32_t* indices, std::uint32_t count)
    : m_count(count)
{
    glCheck(glGenBuffers(1, &m_id));
    glCheck(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
    glCheck(
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(std::uint32_t), indices, GL_DYNAMIC_DRAW));
}

OpenGL41IndexBuffer::~OpenGL41IndexBuffer()
{
    glCheck(glDeleteBuffers(1, &m_id));
}
} // namespace elv
