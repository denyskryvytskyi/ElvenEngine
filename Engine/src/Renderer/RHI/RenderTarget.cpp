#include "RenderTarget.h"

#include "Renderer/Renderer.h"
#include "RendererAPI.h"
#include "Texture.h"

#include "Platform/OpenGL/OpenGLRenderTarget.h"
#include "Platform/OpenGL41/OpenGL41RenderTarget.h"

namespace elv {

void RenderTarget::BindColorTexture(const int slot) const
{
    m_colorTextureAttachment->BindToSlot(slot);
}
std::shared_ptr<Texture> RenderTarget::GetColorTextureAttachment() const
{
    return m_colorTextureAttachment;
}

UniquePtr<RenderTarget> RenderTarget::Create()
{
    switch (Renderer::GetAPI()) {
    case RendererAPI::API::None:
        EL_CORE_ASSERT(false, "RendererAPI::None is currently not supported!");
        return nullptr;
    case RendererAPI::API::OpenGL:
        return MakeUniquePtr<OpenGLRenderTarget>();
    case RendererAPI::API::OpenGL41:
        return MakeUniquePtr<OpenGL41RenderTarget>();
    }

    EL_CORE_ASSERT(false, "Unknown RendererAPI!");
    return nullptr;
}
} // namespace elv
