#include "OpenGL41Texture.h"

#include "Platform/OpenGL41/OpenGL41Check.h"
#include "Platform/OpenGLCommon.h"

#include <glad/gl.h>

namespace elv {

OpenGL41Texture::OpenGL41Texture(std::uint32_t width, std::uint32_t height, std::uint32_t nrChannels)
    : m_width(width)
    , m_height(height)
    , m_bindTarget(GL_TEXTURE_2D)
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
    glCheck(glBindTexture(m_bindTarget, m_id));
    glTexImage2D(m_bindTarget, 0, m_internalFormat, m_width, m_height, 0, m_dataFormat, GL_UNSIGNED_BYTE, nullptr);

    glCheck(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT));
    glCheck(glTexParameteri(m_bindTarget, GL_TEXTURE_WRAP_T, GL_REPEAT));
    glCheck(glTexParameteri(m_bindTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR));
    glCheck(glTexParameteri(m_bindTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
    glCheck(glBindTexture(m_bindTarget, 0));
}

OpenGL41Texture::OpenGL41Texture(std::uint32_t width, std::uint32_t height, const Info& info)
    : m_width(width)
    , m_height(height)
{
    const auto format = OpenGL::GetGLDataFormat(info.InternalFormat);

    m_internalFormat = format.first;
    m_dataFormat = format.second;

    if (info.Multisampled) {
        m_bindTarget = GL_TEXTURE_2D_MULTISAMPLE;

        glCheck(glGenTextures(1, &m_id));

        glCheck(glBindTexture(m_bindTarget, m_id));

        glCheck(glTexImage2DMultisample(m_bindTarget, 4, m_internalFormat, width, height, GL_TRUE));

        glCheck(glBindTexture(m_bindTarget, 0));

    } else {
        m_bindTarget = GL_TEXTURE_2D;

        glCheck(glGenTextures(1, &m_id));
        glCheck(glBindTexture(m_bindTarget, m_id));
        glCheck(glTexImage2D(m_bindTarget, 0, m_internalFormat, m_width, m_height, 0, m_dataFormat, OpenGL::GetGLTextureDataType(info.DataType), nullptr));

        glCheck(glTexParameteri(m_bindTarget, GL_TEXTURE_WRAP_S, OpenGL::GetGLWrappingMode(info.WrapS)));
        glCheck(glTexParameteri(m_bindTarget, GL_TEXTURE_WRAP_R, OpenGL::GetGLWrappingMode(info.WrapR)));
        glCheck(glTexParameteri(m_bindTarget, GL_TEXTURE_WRAP_T, OpenGL::GetGLWrappingMode(info.WrapT)));
        glCheck(glTexParameteri(m_bindTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
        glCheck(glTexParameteri(m_bindTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
        glCheck(glBindTexture(m_bindTarget, 0));
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
    glCheck(glBindTexture(m_bindTarget, m_id));
}

void OpenGL41Texture::Unbind()
{
    glCheck(glActiveTexture(GL_TEXTURE0 + m_slot));
    glCheck(glBindTexture(m_bindTarget, 0));
}

void OpenGL41Texture::SetData(void* data, bool generateMipmap /* = true */)
{
    glCheck(glBindTexture(m_bindTarget, m_id));
    if (generateMipmap) {
        glCheck(glGenerateMipmap(m_bindTarget));
    }
    glTexSubImage2D(m_bindTarget, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
    glCheck(glBindTexture(m_bindTarget, 0));
}

void OpenGL41Texture::SetWrappingMode(const WrappingMode wrappingMode)
{
    auto mode = OpenGL::GetGLWrappingMode(wrappingMode);

    glCheck(glBindTexture(m_bindTarget, m_id));
    glCheck(glTexParameteri(m_bindTarget, GL_TEXTURE_WRAP_S, mode));
    glCheck(glTexParameteri(m_bindTarget, GL_TEXTURE_WRAP_T, mode));
    glCheck(glBindTexture(m_bindTarget, 0));
}

} // namespace elv
