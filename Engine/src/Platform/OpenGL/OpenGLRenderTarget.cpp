#include "OpenGLRenderTarget.h"

#include "Renderer/RHI/Texture.h"
#include "Resources/TextureManager.h"

#include <glad/gl.h>

namespace elv {
OpenGLRenderTarget::~OpenGLRenderTarget()
{
    glDeleteFramebuffers(1, &m_id);
}

void OpenGLRenderTarget::Init(const std::uint32_t width, const std::uint32_t height, bool isMultisampled)
{
    m_isMultisampled = isMultisampled;
    m_size = { width, height };

    glCreateFramebuffers(1, &m_id);
    InitAttachments();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLRenderTarget::Bind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_id);
}

void OpenGLRenderTarget::Unbind() const
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLRenderTarget::BindDraw() const
{
    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_id);
}

void OpenGLRenderTarget::BindRead() const
{
    glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id);
}

void OpenGLRenderTarget::Resize(const std::uint32_t width, const std::uint32_t height)
{
    m_size = { width, height };

    Bind();
    InitAttachments();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGLRenderTarget::Blit(const UniquePtr<RenderTarget>& src)
{
    src->BindRead();
    BindDraw();
    glBlitFramebuffer(0, 0, m_size.Width, m_size.Height, 0, 0, src->GetSize().Width, src->GetSize().Height, GL_COLOR_BUFFER_BIT, GL_NEAREST);
}

void OpenGLRenderTarget::InitAttachments()
{
    {
        Texture::Info info;
        info.Multisampled = m_isMultisampled;

        m_colorTextureAttachment = textures::Load(m_size.Width, m_size.Height, info);
        glNamedFramebufferTexture(m_id, GL_COLOR_ATTACHMENT0, m_colorTextureAttachment->GetId(), 0);
    }

    {
        Texture::Info info;
        info.InternalFormat = Texture::InternalFormat::DepthStencil;
        info.Multisampled = m_isMultisampled;

        m_depthStencilAttachment = textures::Load(m_size.Width, m_size.Height, info);
        glNamedFramebufferTexture(m_id, GL_DEPTH_STENCIL_ATTACHMENT, m_depthStencilAttachment->GetId(), 0);
    }

    if (glCheckNamedFramebufferStatus(m_id, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
        EL_CORE_ERROR("Framebuffer is not complete!");
    }
}
} // namespace elv
