#include "Texture.h"
#include "Renderer/Renderer.h"

#include "Platform/OpenGL/OpenGLTexture.h"
#include "Platform/OpenGL41/OpenGL41Texture.h"

namespace elv {
SharedPtr<Texture> Texture::Create(std::uint32_t width, std::uint32_t height, const Texture::Info& info)
{
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
        EL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return MakeSharedPtr<OpenGLTexture>(width, height, info);
    case RendererAPI::API::OpenGL41:
        return MakeSharedPtr<OpenGL41Texture>(width, height, info);
    }

    EL_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}

SharedPtr<Texture> Texture::Create(std::uint32_t width, std::uint32_t height, std::uint32_t nrChannels)
{
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
        EL_CORE_ASSERT(false, "Renderer API::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return MakeSharedPtr<OpenGLTexture>(width, height, nrChannels);
    case RendererAPI::API::OpenGL41:
        return MakeSharedPtr<OpenGL41Texture>(width, height, nrChannels);
    }
}
} // namespace elv