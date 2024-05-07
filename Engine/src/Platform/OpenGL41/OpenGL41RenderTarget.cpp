#include "Platform/OpenGL41/OpenGL41RenderTarget.h"
#include "Platform/OpenGL41/OpenGL41Check.h"

#include "Renderer/RHI/Texture.h"
#include "Resources/TextureManager.h"

#include <glad/gl.h>

namespace elv {
OpenGL41RenderTarget::~OpenGL41RenderTarget()
{
    glCheck(glDeleteFramebuffers(1, &m_id));
}

void OpenGL41RenderTarget::Init(const std::uint32_t width, const std::uint32_t height, bool isMultisampled)
{
    m_isMultisampled = isMultisampled;
    m_size = { width, height };

    glCheck(glGenFramebuffers(1, &m_id));
    glCheck(glBindFramebuffer(GL_FRAMEBUFFER, m_id));
    InitAttachments();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGL41RenderTarget::Bind() const
{
    glCheck(glBindFramebuffer(GL_FRAMEBUFFER, m_id));
}

void OpenGL41RenderTarget::Unbind() const
{
    glCheck(glBindFramebuffer(GL_FRAMEBUFFER, 0));
}

void OpenGL41RenderTarget::BindDraw() const
{
    glCheck(glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_id));
}

void OpenGL41RenderTarget::BindRead() const
{
    glCheck(glBindFramebuffer(GL_READ_FRAMEBUFFER, m_id));
}

void OpenGL41RenderTarget::Resize(const std::uint32_t width, const std::uint32_t height)
{
    m_size = { width, height };

    Bind();
    InitAttachments();
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void OpenGL41RenderTarget::Blit(const UniquePtr<RenderTarget>& src)
{
    src->BindRead();
    BindDraw();
    glCheck(glBlitFramebuffer(0, 0, m_size.Width, m_size.Height, 0, 0, src->GetSize().Width, src->GetSize().Height, GL_COLOR_BUFFER_BIT, GL_NEAREST));
}

void OpenGL41RenderTarget::InitAttachments()
{
    {
        Texture::Info info;
        info.Multisampled = m_isMultisampled;

        m_colorTextureAttachment = textures::Load(m_size.Width, m_size.Height, info);
        glCheck(
            glFramebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_COLOR_ATTACHMENT0,
                GL_TEXTURE_2D,
                m_colorTextureAttachment->GetId(),
                0));
    }

    {
        Texture::Info info;
        info.InternalFormat = Texture::InternalFormat::DepthStencil;
        info.Multisampled = m_isMultisampled;

        m_depthStencilAttachment = textures::Load(m_size.Width, m_size.Height, info);
        // glNamedFramebufferTexture(m_id, GL_DEPTH_STENCIL_ATTACHMENT, m_depthStencilAttachment->GetId(), 0);

        glCheck(
            glFramebufferTexture2D(
                GL_FRAMEBUFFER,
                GL_DEPTH_STENCIL_ATTACHMENT,
                GL_TEXTURE_2D,
                m_depthStencilAttachment->GetId(),
                0));
    }

    auto status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (status != GL_FRAMEBUFFER_COMPLETE) {
        EL_CORE_ERROR("Framebuffer is not complete!");
    }
}
} // namespace elv
