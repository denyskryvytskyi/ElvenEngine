#include "OpenGL41Texture.h"

#include "Platform/OpenGL41/OpenGL41Check.h"

#include <glad/gl.h>

namespace elv {

GLenum GetGLWrappingMode(const Texture::WrappingMode wrappingMode)
{
    switch (wrappingMode) {
    case Texture::WrappingMode::Repeat:
        return GL_REPEAT;
    case Texture::WrappingMode::MirroredRepeat:
        return GL_MIRRORED_REPEAT;
    case Texture::WrappingMode::ClampToBorder:
        return GL_CLAMP_TO_BORDER;
    }

    return GL_CLAMP_TO_EDGE;
}

std::pair<GLenum, GLenum> GetGLDataFormat(const Texture::InternalFormat format)
{
    GLenum internalFormat = GL_RGBA8;
    GLenum dataFormat = GL_RGBA;

    switch (format) {
    case Texture::InternalFormat::RGB8:
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
        break;
    case Texture::InternalFormat::R8:
        internalFormat = GL_R8;
        dataFormat = GL_RED;
        break;
    case Texture::InternalFormat::DepthStencil:
        internalFormat = GL_DEPTH24_STENCIL8;
        dataFormat = GL_DEPTH_STENCIL;
        break;
    }

    return std::make_pair(internalFormat, dataFormat);
}

OpenGL41Texture::OpenGL41Texture(std::uint32_t width, std::uint32_t height, std::uint32_t nrChannels /* = 3 */)
    : m_width(width)
    , m_height(height)
{

    if (nrChannels == 4) {
        m_internalFormat = GL_RGBA8;
        m_dataFormat = GL_RGBA;
    } else if (nrChannels == 3) {
        m_internalFormat = GL_RGB8;
        m_dataFormat = GL_RGB;
    } else {
        m_internalFormat = GL_R8;
        m_dataFormat = GL_RED;
    }

    glCheck(glGenTextures(1, &m_id));
    glCheck(glBindTexture(GL_TEXTURE_2D, m_id));
    glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_dataFormat, GL_UNSIGNED_BYTE, nullptr);

    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    glCheck(glBindTexture(GL_TEXTURE_2D, 0));
}

OpenGL41Texture::OpenGL41Texture(std::uint32_t width, std::uint32_t height, const Info& info)
{
    const auto format = GetGLDataFormat(info.InternalFormat);

    if (info.Multisampled) {
        // glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_id);
        // glTextureStorage2DMultisample(m_id, 4, format.first, width, height, GL_TRUE);
    } else {
        glCheck(glGenTextures(1, &m_id));
        glCheck(glBindTexture(GL_TEXTURE_2D, m_id));
        glTexImage2D(GL_TEXTURE_2D, 0, m_internalFormat, m_width, m_height, 0, m_dataFormat, GL_UNSIGNED_BYTE, nullptr);

        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GetGLWrappingMode(info.WrapS)));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GetGLWrappingMode(info.WrapR)));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GetGLWrappingMode(info.WrapT)));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        glCheck(glBindTexture(GL_TEXTURE_2D, 0));
    }
}

OpenGL41Texture::~OpenGL41Texture()
{
    glCheck(glDeleteTextures(1, &m_id));
}

void OpenGL41Texture::BindToSlot(std::uint32_t slot)
{
    m_slot = slot;
    glCheck(glActiveTexture(GL_TEXTURE0 + slot));
    glCheck(glBindTexture(GL_TEXTURE_2D, m_id));
}

void OpenGL41Texture::Unbind()
{
    glCheck(glActiveTexture(GL_TEXTURE0 + m_slot));
    glCheck(glBindTexture(GL_TEXTURE_2D, 0));
}

void OpenGL41Texture::SetData(void* data, bool generateMipmap /* = true */)
{
    glCheck(glBindTexture(GL_TEXTURE_2D, m_id));
    if (generateMipmap) {
        glCheck(glGenerateMipmap(GL_TEXTURE_2D));
    }
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
    glCheck(glBindTexture(GL_TEXTURE_2D, 0));
}

void OpenGL41Texture::SetWrappingMode(const WrappingMode wrappingMode)
{
    auto mode = GetGLWrappingMode(wrappingMode);

    glCheck(glBindTexture(GL_TEXTURE_2D, m_id));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, mode));
    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, mode));
    glCheck(glBindTexture(GL_TEXTURE_2D, 0));
}

} // namespace elv
