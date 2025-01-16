#include "OpenGLCubemap.h"

#include <glad/gl.h>

namespace elv {

OpenGLCubemap::OpenGLCubemap(const std::vector<TextureManager::LoadedTextureInfo>& texturesInfo)
{
    // Generate the cubemap texture
    glCreateTextures(GL_TEXTURE_CUBE_MAP, 1, &m_id);

    // Specify the storage for the cubemap texture
    glTextureStorage2D(m_id, 1, GL_RGB8, texturesInfo[0].width, texturesInfo[0].height);

    // Upload the texture data for each face of the cubemap
    for (unsigned int i = 0; i < texturesInfo.size(); ++i) {
        if (texturesInfo[i].data) {
            glTextureSubImage3D(
                m_id,
                0,                      // Level of detail
                0, 0, i,                // Offset (x, y, layer)
                texturesInfo[i].width,  // Width of the face
                texturesInfo[i].height, // Height of the face
                1,                      // Depth (1 for single-layer 2D)
                GL_RGB,                 // Format of input data
                GL_UNSIGNED_BYTE,       // Data type of input data
                texturesInfo[i].data    // Pointer to the texture data
            );
        }
    }

    // Set texture parameters using DSA
    glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTextureParameteri(m_id, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

void OpenGLCubemap::Bind()
{
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_id);
}

} // namespace elv
