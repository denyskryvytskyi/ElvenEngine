#include "Texture2D.h"
#include "Renderer/Renderer.h"
#include "Platform/OpenGL/OpenGLTexture2D.h"

namespace Elven
{
    SharedPtr<Texture2D> Texture2D::Create(const char* filepath)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL:
            return CreateSharedPtr<OpenGLTexture2D>(filepath);
        }
        
        EL_CORE_ASSERT(false, "Unknown Renderer API!");

        return nullptr;
    }

    SharedPtr<Texture2D> Texture2D::Create(uint32_t width, uint32_t height)
    {
        switch (Renderer::GetAPI())
        {
        case RendererAPI::API::OpenGL:
            return CreateSharedPtr<OpenGLTexture2D>(width, height);
        }

        EL_CORE_ASSERT(false, "Unknown Renderer API!");

        return nullptr;
    }

}
