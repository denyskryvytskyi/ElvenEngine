#include "OpenGLTexture.h"

#include "Platform/OpenGLCommon.h"

#include <glad/gl.h>

namespace elv {
OpenGLTexture::OpenGLTexture(std::uint32_t width, std::uint32_t height, std::uint32_t nrChannels /* = 3 */)
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

    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    glTextureStorage2D(m_id, 1, m_internalFormat, m_width, m_height);

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

OpenGLTexture::OpenGLTexture(std::uint32_t width, std::uint32_t height, const Info& info)
{
    const auto format = OpenGL::GetGLDataFormat(info.InternalFormat);

    if (info.Multisampled) {
        glCreateTextures(GL_TEXTURE_2D_MULTISAMPLE, 1, &m_id);
        glTextureStorage2DMultisample(m_id, 4, format.first, width, height, GL_TRUE);
    } else {
        glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
        glTextureStorage2D(m_id, 1, format.first, width, height);

        const auto mode = OpenGL::GetGLWrappingMode(info.WrapR);
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_R, OpenGL::GetGLWrappingMode(info.WrapR));
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, OpenGL::GetGLWrappingMode(info.WrapS));
        glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, OpenGL::GetGLWrappingMode(info.WrapT));
        glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    }
}

OpenGLTexture::~OpenGLTexture()
{
    glDeleteTextures(1, &m_id);
}

void OpenGLTexture::BindToSlot(std::uint32_t slot)
{
    m_slot = slot;
    glBindTextureUnit(slot, m_id);
}

void OpenGLTexture::Unbind()
{
    glBindTextureUnit(m_slot, 0);
}

void OpenGLTexture::SetData(void* data, bool generateMipmap /* = true */)
{
    if (generateMipmap) {
        glGenerateTextureMipmap(m_id);
    }
    glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
}

void OpenGLTexture::SetWrappingMode(const WrappingMode wrappingMode)
{
    auto mode = OpenGL::GetGLWrappingMode(wrappingMode);

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, mode);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, mode);
}

} // namespace elv
