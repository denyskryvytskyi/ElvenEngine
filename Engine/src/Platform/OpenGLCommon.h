#pragma once

#include <glad/gl.h>

#include "Renderer/RHI/Buffer.h"
#include "Renderer/RHI/Texture.h"
#include "Renderer/RenderTopology.h"

namespace elv {
namespace OpenGL {

GLenum GetTopology(const RenderTopology topology);

GLenum GetGLWrappingMode(const Texture::WrappingMode wrappingMode);

std::pair<GLenum, GLenum> GetGLDataFormat(const Texture::InternalFormat format);

GLenum BufferAttributeTypeToOpenGLBaseType(BufferAttributeType type);

} // namespace OpenGL
} // namespace elv