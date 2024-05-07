#include "OpenGLCommon.h"

namespace elv {
namespace OpenGL {

GLenum GetTopology(const RenderTopology topology)
{
    switch (topology) {
    case RenderTopology::TriangleStrip:
        return GL_TRIANGLE_STRIP;
    case RenderTopology::Lines:
        return GL_LINES;
    case RenderTopology::LineStrip:
        return GL_LINE_STRIP;
    }

    return GL_TRIANGLES;
}

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

GLenum BufferAttributeTypeToOpenGLBaseType(BufferAttributeType type)
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

} // namespace OpenGL
} // namespace elv