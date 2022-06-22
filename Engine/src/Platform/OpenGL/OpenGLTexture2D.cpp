#include "OpenGLTexture2D.h"
#include "stb_image.h"

#include <glad/gl.h>

namespace Elven {

OpenGLTexture2D::OpenGLTexture2D(const std::string& filepath)
{
    stbi_set_flip_vertically_on_load(true);

    int width, height, nrChannels;
    unsigned char* data = stbi_load(filepath.c_str(), &width, &height, &nrChannels, 0);

    if (data == nullptr) {
        EL_CORE_ASSERT(false, "Failed to load texture!");
        return;
    }

    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);

    GLenum internalFormat, dataFormat;
    if (nrChannels == 4) {
        internalFormat = GL_RGBA8;
        dataFormat = GL_RGBA;
    } else {
        internalFormat = GL_RGB8;
        dataFormat = GL_RGB;
    }

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTextureStorage2D(m_id, 1, internalFormat, width, height);
    glGenerateTextureMipmap(m_id);
    glTextureSubImage2D(m_id, 0, 0, 0, width, height, dataFormat, GL_UNSIGNED_BYTE, data);
}

OpenGLTexture2D::OpenGLTexture2D(std::uint32_t width, std::uint32_t height)
    : m_width(width)
    , m_height(height)
    , m_internalFormat(GL_RGBA8)
    , m_dataFormat(GL_RGBA)
{
    glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
    glTextureStorage2D(m_id, 1, m_internalFormat, m_width, m_height);

    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void OpenGLTexture2D::BindToUnit(std::uint32_t unit)
{
    glBindTextureUnit(unit, m_id);
}

void OpenGLTexture2D::SetData(void* data)
{
    glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, m_dataFormat, GL_UNSIGNED_BYTE, data);
}

} // namespace Elven
