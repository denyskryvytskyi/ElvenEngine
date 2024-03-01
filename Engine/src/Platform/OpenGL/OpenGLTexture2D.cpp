#include "OpenGLTexture2D.h"

#include <glad/gl.h>

namespace elv {

GLenum GetGLWrappingMode(const TextureWrappingMode wrappingMode)
{
    switch (wrappingMode) {
    case TextureWrappingMode::Repeat:
        return GL_REPEAT;
    case TextureWrappingMode::MirroredRepeat:
        return GL_MIRRORED_REPEAT;
    case TextureWrappingMode::ClampToBorder:
        return GL_CLAMP_TO_BORDER;
    }

    return GL_CLAMP_TO_EDGE;
}

OpenGLTexture2D::OpenGLTexture2D(std::uint32_t width, std::uint32_t height, std::uint32_t nrChannels /* = 3 */)
    : m_width(width)
    , m_height(height)
{
    if (nrChannels == 4) {
        m_internalFormat = GL_RGBA8;
        m_dataFormat = GL_RGBA;
    } else if (nrChannels == 3) {
        m_internalFormat = GL_RGB8;
        m_dataFormat = GL_RGB;
    } else { // for text textures
        m_internalFormat = GL_R8;
        m_dataFormat = GL_RED;
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    glTextureStorage2D(m_id, 1, m_internalFormat, m_width, m_height);

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

OpenGLTexture2D::~OpenGLTexture2D()
{
    glDeleteTextures(1, &m_id);
}

void OpenGLTexture2D::BindToUnit(std::uint32_t unit)
{
    glBindTextureUnit(unit, m_id);
}

void OpenGLTexture2D::SetData(void* data, bool generateMipmap /* = true */)
{
    if (generateMipmap) {
        glGenerateTextureMipmap(m_id);
    }
    glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
}

void OpenGLTexture2D::SetWrappingMode(const TextureWrappingMode wrappingMode)
{
    auto mode = GetGLWrappingMode(wrappingMode);

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, mode);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, mode);
}

} // namespace elv
